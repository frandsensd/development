/*
 simple_int.c -- Simple driver for investigating interrupt handling
        
    Copyright (C) 2007 to 2012 Doug Abbott
    
    This source code file may be freely used, adapted,
    and redistributed in source or binary form, provided
    the current author is acknowledged.
    
    No warranty is expressed or implied, nor is any responsibility
    assumed for errors or for fitness for any purpose.
 */

/*
   This version is specifically adapted to the ARM9 memory-mapped I/O space of
   the Samsung S3C2440 processor.
 */

#include <linux/module.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/poll.h>

#include <linux/sched.h>
#include <linux/delay.h>    /* udelay */
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/slab.h>

#include <mach/regs-gpio.h>
#include <mach/regs-irq.h>
#include <mach/gpio-fns.h>
#include <mach/hardware.h>

#include <asm/io.h>

#define IRQ_FLAGS IRQF_TRIGGER_RISING

#ifdef DEBUG
#   define PDEBUG(fmt, args...) printk (KERN_INFO "simple_int: " fmt, ## args)
#else
#   define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

static int major = 0;   /* dynamic by default */
module_param(major, int, 0);

/* The interrupt line is undefined by default. */
static int irq = -1;
module_param(irq, int, 0);

static unsigned int bit = 0;   /* select at load time which bit of GPF drives interrupt */
module_param(bit, int, 0);

static int probe = 0;   /* select at load time how to probe irq line */
module_param(probe, int, 0);

static int wq = 0;  /* select at load time whether a workqueue is used */
module_param(wq, int, 0);

static int tasklet = 0; /* select whether a tasklet is used */
module_param(tasklet, int, 0);

static int share = 0;   /* select at load time whether install a shared irq */
module_param(share, int, 0);

MODULE_AUTHOR ("Doug Abbott");
MODULE_LICENSE ("GPL");

unsigned long text_buffer = 0;
unsigned long volatile text_head;
volatile unsigned long text_tail;
DECLARE_WAIT_QUEUE_HEAD (read_queue);

static inline void incr_bp (volatile unsigned long *index, int delta)
/*
   Atomicly increment an index into text_buffer
*/
{
    unsigned long new = *index + delta;
    barrier();  /* Don't optimize these two together */
    *index = (new >= (text_buffer + PAGE_SIZE)) ? text_buffer : new;
}

/* Basic driver functions */

int int_open (struct inode *inode, struct file *filp)
{
    return 0;
}

int int_release (struct inode *inode, struct file *filp)
{
    return 0;
}

ssize_t int_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int count0;

    while (text_head == text_tail)
    {
        if (wait_event_interruptible (read_queue, (text_head != text_tail)))
            return -ERESTARTSYS; 
    } 
    /* count0 is the number of readable data bytes */
    count0 = text_head - text_tail;
    if (count0 < 0) /* wrapped */
        count0 = text_buffer + PAGE_SIZE - text_tail;
    if (count0 < count) count = count0;

    if (copy_to_user (buf, (char *)text_tail, count))
        return -EFAULT;

    incr_bp (&text_tail, count);
    return count;
}

ssize_t int_write (struct file *filp, const char __user *buf, size_t count,
        loff_t *f_pos)
{
    int written = 0;

    while (written < count)
	{
        s3c2410_gpio_setpin (S3C2410_GPF(bit), (written++ & 1));
	}
    *f_pos += count;
    return written;
}

irqreturn_t my_interrupt (int irq_in, void *dev_id)
/*
    Basic interrupt service routine.  Get the current time, write it to
    the circular buffer and wake up any reading process.
*/
{
    struct timeval tv;
    int written;

    do_gettimeofday (&tv);

/* Write a 16 byte record. Assume PAGE_SIZE is a multiple of 16 */
    written = sprintf ((char *)text_head,"%08u.%06u\n",
            (int)(tv.tv_sec % 100000000), (int)(tv.tv_usec));
    incr_bp (&text_head, written);
    wake_up_interruptible (&read_queue); /* awake any reading process */
    return IRQ_HANDLED;
}

irqreturn_t sh_interrupt (int irq_in, void *dev_id)
/*
    Basic shared interrupt service routine.  Like the one above except
    that it returns immediately if it wasn't our interrupt.
*/
{
    int written;
    struct timeval tv;

    /* If it wasn't us, return immediately */
    if (!(s3c2410_gpio_getpin (S3C2410_GPF(bit))))
        return IRQ_NONE;
    
    do_gettimeofday (&tv);
    written = sprintf ((char *)text_head,"%08u.%06u\n",
            (int)(tv.tv_sec % 100000000), (int)(tv.tv_usec));
    incr_bp (&text_head, written);
    wake_up_interruptible (&read_queue); /* awake any reading process */
    return IRQ_HANDLED;
}

/*
 * The following functions are equivalent to the previous two,
 * but split in top and bottom half. First, a few needed variables
 */

#define NR_TIMEVAL 512 /* length of the array of time values */

struct timeval tv_data[NR_TIMEVAL]; /* too lazy to allocate it */
volatile struct timeval *tv_head=tv_data;
volatile struct timeval *tv_tail=tv_data;

int wq_count = 0;

static inline void incr_tv (volatile struct timeval **tvp)
/*
   Increment a circular buffer pointer in a way that nobody sees
   an intermediate value.
*/
{
    if (*tvp == (tv_data + NR_TIMEVAL - 1))
        *tvp = tv_data;  /* Wrap */
    else
        (*tvp)++;
}

void do_bottom_half (unsigned long unused)
/*
     The bottom half reads the tv buffer, filled by the top half,
     and prints it to the circular text buffer, which is then consumed
     by reading processes
*/
{
    int savecount = wq_count, written;
    wq_count = 0; /* we have already been removed from the queue */

    /* First write the number of interrupts that occurred before this bh */
    written = sprintf ((char *)text_head,"bh after %6i\n",savecount);
    incr_bp (&text_head, written);

    /*
     * Then, write the time values. Write exactly 16 bytes at a time,
     * so it aligns with PAGE_SIZE
     */
    do
    {
        written = sprintf ((char *)text_head,"%08u.%06u\n",
                (int)(tv_tail->tv_sec % 100000000),
                (int)(tv_tail->tv_usec));
        incr_bp (&text_head, written);
        incr_tv (&tv_tail);
    }
    while (tv_tail != tv_head);

    wake_up_interruptible (&read_queue); /* awake any reading process */
}

static struct work_struct workq;

irqreturn_t wq_interrupt (int irq_in, void *dev_id)
/*
    Workqueue top half, ISR
*/
{
    /* Grab the current time information. */
    do_gettimeofday ((struct timeval *) tv_head);
    incr_tv (&tv_head);

    /* Queue the bh. Don't worry about multiple enqueueing */
    schedule_work (&workq);

    wq_count++; /* record that an interrupt arrived */
    return IRQ_HANDLED;
}

/* Set up our tasklet if we're doing that. */
DECLARE_TASKLET(hw_tasklet, do_bottom_half, 0);

irqreturn_t tl_interrupt (int irq_in, void *dev_id)
/*
   Tasklet top half, ISR
*/
{
    do_gettimeofday ((struct timeval *) tv_head); /* cast to stop 'volatile' warning */
    incr_tv (&tv_head);
    tasklet_schedule (&hw_tasklet);
    
    wq_count++; /* record that an interrupt arrived */
    return IRQ_HANDLED;
}

void pulse_bit (int bit)
/*
    'bit' is a GPIO bit.  Create a 5 usec pulse on it.
*/
{
    s3c2410_gpio_setpin (bit, 1);       // generate an interrupt
    udelay (5);                         // give it some time
    s3c2410_gpio_setpin (bit, 0);       // set it back
}

void kernel_probe (void)
/*
    Uses kernel helper functions to probe for device's interrupt.  Possible interrupts
    are EINT1, EINT2, or EINT3 driven by GPF bits 0, 1, or 2.

    This function isn't working yet.
*/
{
    int count = 0;
    do
    {
        unsigned long mask;

        mask = probe_irq_on();
        pulse_bit (S3C2410_GPF(bit));       // generate an interrupt
        irq = probe_irq_off (mask);

        if (irq == 0)
        { /* none of them? */
            printk (KERN_INFO "simple_int: no irq reported by kernel_probe\n");
            irq = -1;
        }
        /*
         * if more than one line has been activated, the result is
         * negative. We should service the interrupt (no need for lpt port)
         * and loop over again. Loop at most five times, then give up
         */
    }
    while (irq < 0 && ++count < 6);  
    if (irq < 0)
        printk (KERN_INFO "simple_int: kernel_probe failed %i times, giving up\n", count - 1);
	else
        printk (KERN_INFO "simple_int: kernel_probe found interrupt %i after %i tries\n", irq, count);
}

irqreturn_t probing_isr (int irq_in, void *dev_id)
/*
    ISR for self-probing
*/
{
    if (irq == 0) irq = irq_in;         /* found */
    if (irq != irq_in) irq = -irq_in;   /* ambiguous */
    
    return IRQ_HANDLED;
}

void self_probe (void)
/*
    DIY interrupt probe
*/
{
    int trials[] = {IRQ_EINT1, IRQ_EINT2, IRQ_EINT3, 0};
    int tried[]  = {0, 0, 0, 0};
    int i, count = 0;
/*
     install the probing handler for all possible lines. Remember
     the result (0 for success, or -EBUSY) in order to only free
     what has been acquired
*/
    for (i = 0; trials[i]; i++)
        tried[i] = request_irq (trials[i], probing_isr,
                IRQF_DISABLED, "simple_probe", NULL);

    do
    {
        irq = 0; /* none got, yet */
        pulse_bit (S3C2410_GPF(bit));       // generate an interrupt

        /* the value has been set by the handler */
        if (irq == 0) /* none of them? */
            printk (KERN_INFO "simple_int: no irq reported by self_probe\n");
/*
    If more than one line has been activated, the result is negative.
    We should service the interrupt and loop over again. Do it at most 5 times
*/
    }
    while (count++ < 5 && irq <= 0);

/* end of loop, uninstall the handler */
    for (i = 0; trials[i]; i++)
        if (tried[i] == 0)
            free_irq (trials[i], NULL);

    if (irq < 0)
        printk (KERN_INFO "simple_int: self_probe failed %i times, giving up\n", count);
	else
        printk (KERN_INFO "simple_int: self_probe found interrupt %i after %i tries.\n", irq, count);
}

/* Finally, init and cleanup */

struct file_operations int_fops = {
    .owner   = THIS_MODULE,
    .read    = int_read,
    .write   = int_write,
    .open    = int_open,
    .release = int_release,
};

static void common_cleanup (void)
{
    free_page (text_buffer);
    unregister_chrdev (major, "simple_int");
    s3c2410_gpio_setpin (S3C2410_GPF(bit), 0);       // leave the bit 0
}

int int_init (void)
{
    int result;
    
    /* Simple sanity check */
    if (wq + tasklet > 1)
    {
        printk (KERN_INFO "simple_int: can't use work queue and tasket simultaneously\n");
        return -EINVAL;
    }
    /* Another sanity check */
    if (bit > 2)
    {
        printk (KERN_INFO "simple_int: driving bit must be 0 to 2\n");
        return -EINVAL;
    }
/*  Set the driving bit of GPIO port F as output and set it to 0
    Don't have to do this because it's the boot up default */
//    s3c2410_gpio_cfgpin (S3C2410_GPF(bit), 1);
//    s3c2410_gpio_setpin (S3C2410_GPF(bit), 0);

/* Register the device using the 2.4 style registration */
    result = register_chrdev (major, "simple_int", &int_fops);
    if (result < 0)
    {
        printk (KERN_INFO "simple_int: can't get major number\n");
//        release_region (port, NR_PORTS);
        return result;
    }
    if (major == 0) major = result; /* dynamic */

    text_buffer = __get_free_page (GFP_KERNEL); /* never fails? */  /* FIXME */
    text_head = text_tail = text_buffer;
    /*
     * Fill the workqueue structure in case we want to use it.
     */
    INIT_WORK(&workq, (work_func_t) do_bottom_half);
    /*
     * Now find the interrupt using either kernel-based
     * autodetection, DIY detection or default number
     */

    if (irq < 0 && probe == 1)
        kernel_probe();

    if (irq < 0 && probe == 2)
        self_probe();

    if (irq < 0) /* not yet specified: force the default on */
        irq = IRQ_EINT2;
    /*
        Register correct interrupt handler
    */
    do
    {
        if (tasklet)
        {
            result = request_irq (irq, tl_interrupt, IRQ_FLAGS, "simple_int-bh", NULL);
			printk (KERN_INFO "simple_int: using tasklet bottom half\n");
            break;
        }
        if (wq)
        {
            result = request_irq (irq, wq_interrupt, IRQ_FLAGS, "simple_int-bh", NULL);
			printk (KERN_INFO "simple_int: using workqueue bottom half\n");
            break;
        }
        if (share)
        {
            result = request_irq (irq, sh_interrupt, IRQF_SHARED | IRQ_FLAGS,  "simple_int-sh", sh_interrupt);
            break;
        }
        result = request_irq (irq, my_interrupt, IRQ_FLAGS, "simple_int", NULL);
    }
    while (0);
    
    if (result)
    {
        printk (KERN_INFO "simple_int: can't get assigned irq %i\n", irq);
        common_cleanup();
        return -ENODEV;
    }
    else printk (KERN_INFO "simple_int: irq %i registered\n", irq);
    return 0;
}

void int_cleanup (void)
{
        if (!share)
            free_irq (irq, NULL);
        else
            free_irq (irq, sh_interrupt);

/* Make sure we don't leave work queue/tasklet functions running */
    if (tasklet)
        tasklet_disable (&hw_tasklet);
    if (wq)
        flush_scheduled_work();

    common_cleanup();
}

module_init(int_init);
module_exit(int_cleanup);
