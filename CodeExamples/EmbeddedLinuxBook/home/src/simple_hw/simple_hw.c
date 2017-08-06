/*
 simple_hw.c -- Simple driver for exercising hardware

    Copyright (C) 2009 to 2012 Doug Abbott

    This source code file may be freely used, adapted,
    and redistributed in source or binary form, provided
    the current author is acknowledged.

    No warranty is expressed or implied, nor is any responsibility
    assumed for errors or for fitness for any purpose.
 */

/*
 * This version is specifically adapted to the ARM9 memory-mapped
 * I/O space of the Samsung S3C2440 processor.
 */

#include <linux/module.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/ioport.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#include <asm/io.h>

#ifdef DEBUG
#   define PDEBUG(fmt, args...) printk (KERN_INFO "simple_hw: " fmt, ## args)
#else
#   define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

static int major = 0;	/* dynamic by default */
module_param(major, int, S_IRUGO);

/* The following parameters are writable from sysfs because it makes sense
 * to be able to change them at run time
 */
static int size = 4;	/* transfer size: 1, 2, or 4 bytes */
module_param(size, int, S_IRUGO | S_IWUSR);

/* Offset within 4-byte register.  Default exposes LED bits */
static unsigned int offset = 0;
module_param(offset, int, S_IRUGO | S_IWUSR);

/* Default base is GPIO Controller B, GPB. */
static unsigned long pbase = 0x56000010;        // physical base address
static unsigned long vbase;                     // virtual base address
module_param(pbase, long, S_IRUGO | S_IWUSR);

MODULE_AUTHOR ("Doug Abbott");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "simple_hw"

/* Basic driver functions */

int hw_open (struct inode *inode, struct file *filp)
{
	
	return 0;
}

int hw_release (struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t hw_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int retval, minor = iminor (filp->f_dentry->d_inode);
    void *address = (void *) vbase + minor*4 + offset;
    unsigned char *kbuf;
    void *ptr;

    if (count > size)
    {
        count &= ~(size - 1);
        retval = count;
    }
    else
    {
        retval = count;    // report what the user wants to see
        count = size;
    }
    kbuf = kmalloc (count, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;
    ptr = kbuf;

    PDEBUG ("read. address = %p, count = %d\n", address, count);
    while (count)
    {
        switch (size)
        {
            case 1:
                *((uint8_t *) ptr) = ioread8 (address);
                break;

            case 2:
                *((uint16_t *) ptr) = ioread16 (address);
                break;

            case 4:
                *((uint32_t *) ptr) = ioread32 (address);
                break;
        }
        ptr += size;
        count -= size;
    }

	if (copy_to_user (buf, kbuf, retval))
		retval = -EFAULT;
	kfree (kbuf);
	return retval;
}

ssize_t hw_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
/*
    The write function is optimized for the LEDs connected to GPB.  The data
    passed in is inverted so a 1 means the LED is on and is shifted left 5 bits
    so that the least significant bits represent the 4 LEDs.
*/
{
	int  retval, minor = iminor(filp->f_dentry->d_inode);
	void *address = (void *) vbase + minor*4 + offset;
	unsigned char *kbuf;
    unsigned int *ptr;

	if (count > size)
	{
	    count &= ~(size - 1);
	    retval = count;
	}
	else
	{
	    retval = count;    // report what the user wants to see
	    count = size;
	}
	kbuf = kmalloc (count, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user (kbuf, buf, retval))
	{
		retval = -EFAULT;
		goto write_end;
	}
	ptr = (unsigned int *) kbuf;

    while (count)
    {
        *ptr = ~*ptr << 5;
        switch (size)
        {
            case 1:
                iowrite8 (*ptr, address);
// Doing it directly, the "non-portable" way
//                *((uint8_t *) address) = *((uint8_t *) ptr);
                break;

            case 2:
                iowrite16 (*ptr, address);
                break;

            case 4:
                iowrite32 (*ptr, address);
                break;
        }
        ptr += size;
        count -= size;
    }
write_end:
	kfree (kbuf);
	return retval;
}

struct file_operations hw_fops = {
	.owner	 = THIS_MODULE,
	.read	 = hw_read,
	.write	 = hw_write,
	.open	 = hw_open,
	.release = hw_release,
};

/* Finally, init and cleanup */

static inline void release_ports (void)
/*
    Needed by both init and cleanup
*/
{
    iounmap ((void __iomem *) vbase);
    release_mem_region (pbase, NR_PORTS*4);
}

struct cdev *cdev_hw;
dev_t dev;

static void invalid_offset (void)
{
    printk (KERN_INFO "simple_hw: invalid offset %d for size %d\n", offset, size);
}

int hw_init (void)
{
	int result;

/* Sanity-check the module parameters */
	if (offset > 3)
	{
	    printk (KERN_INFO "simple_hw: invalid offset value %d\n", offset);
	    return -EINVAL;
	}
	switch (size)
	{
	    case 1:    // all offsets are valid
	        break;

	    case 2:
	        if (offset & 1)
	        {
	            invalid_offset();
	            return -EINVAL;
	        }
	        break;

	    case 4:
            if (offset)
            {
                invalid_offset();
                return -EINVAL;
            }
            break;

	    default:
	        printk (KERN_INFO "simple_hw: invalid size value %d\n", size);
	        return -EINVAL;

	}
	
/* Get our needed resources. */
	if (!request_mem_region (pbase, NR_PORTS*4, DEVICE_NAME))
    {
		printk (KERN_INFO "simple_hw: can't get I/O mem address 0x%lx\n",
				pbase);
		return -ENODEV;
	}
	/* also, ioremap it */
	vbase = (unsigned long) ioremap (pbase, NR_PORTS*4);

/* Get a range of minor numbers */
    if (major)
    {
        dev = MKDEV(major, 0);
        result = register_chrdev_region (dev, NR_PORTS, DEVICE_NAME);
    }
    else
    {
        result = alloc_chrdev_region (&dev, 0, NR_PORTS, DEVICE_NAME);
        major = MAJOR(dev);
    }
    if (result < 0)
    {
        printk (KERN_INFO "simple_hw: can't get major %d\n", major);
	    release_ports();
        return result;
    }
	cdev_hw = cdev_alloc();
	cdev_hw->ops = &hw_fops;
	cdev_hw->owner = THIS_MODULE;

	result = cdev_add (cdev_hw, dev, NR_PORTS);
	if (result)
	{
	    printk (KERN_INFO "simple_hw: couldn't add device\n");
		unregister_chrdev_region (dev, NR_PORTS);
	    release_ports();
	    return result;
	}
/*
 * This is a hack for the LEDs to turn them off
 */
	result = ioread16 (vbase + 0x4);
	iowrite16 ((result | 0x1c0), vbase + 0x4);

	printk (KERN_INFO "simple_hw loaded: vbase = %lx, offset = %d, size = %d\n",
	        vbase, offset, size);
	return 0;
}
module_init(hw_init);

void hw_cleanup (void)
{
	cdev_del (cdev_hw);
    unregister_chrdev_region (dev, NR_PORTS);
    release_ports();
}

module_exit(hw_cleanup);
