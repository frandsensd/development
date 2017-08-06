#include "MmapGpio.h"
#include <assert.h>

/*******************************************************************
 * Default constructor....
 * Simply calls mapRegAddri() function to map the physical addresses
 * of the GPIO registers into local process memory
 *
 * Parameters - None
 * Return Value - None
 *******************************************************************/
MmapGpio::MmapGpio()
{
    gpioControllerRegister =
        MapRegisterAddressSpaceToMemoryMap( gpioControllerBaseAddr );
}

/*******************************************************************
 * Destructor - unmaps GPIO registers (physical  memory)  from
 * process memoy
 *
 * Parameters - None
 * Return Value - None
 ******************************************************************/
MmapGpio::~MmapGpio()
{
    //unmap GPIO registers (physicalmemory)  from process memoy
    int result =
        munmap( (void*)gpioControllerRegister,
                gpioControllerRegisterMapSizeBytes );
    if(result < 0)
    {
        perror("munmap (gpio) failed");
        throw GPIO_EXCEPTION_MEMORY_UNMAP_FAILED;
    }
}

/*******************************************************************
 * Retrieves the GPIO controlled base address used for all the
 * communication.
 *
 * Parameters - None
 * Return Value - address
 * 0x20200000 indicates binary is compiled for RPi
 * 0x3F200000 indicates binary is compiled for RPi2.
 ******************************************************************/
uint32_t GetGpioControllerBaseAddress()
{
    return gpioControllerBaseAddr;
}

/*******************************************************************
 * Gets the direction of a pin
 * Pins are divided in groups of ten with the exception of pins 50-53
 * as registers denoted GPFSEL0-5. Within each register we have these
 * definitions:
 * pin 0: bits  2-0  000=input, 001=output.
 * pin 1: bits  5-3  000=input, 001=output.
 * pin 2: bits  8-6  000=input, 001=output.
 * pin 3: bits 11-9  000=input, 001=output.
 * pin 4: bits 14-12 000=input, 001=output.
 * pin 5: bits 17-15 000=input, 001=output.
 * pin 6: bits 20-18 000=input, 001=output.
 * pin 7: bits 23-21 000=input, 001=output.
 * pin 8: bits 26-24 000=input, 001=output.
 * pin 9: bits 29-27 000=input, 001=output.
 * Hence lsb = pin#*3. If lsb is set, pin is output else input.
 *
 * Parameters
 *  pinnum:       GPIO pin number as per the RPI's BCM2835's standard
 *                ranging from 0-53.
 * Return Value:  Pin direction per GPIO_PIN_DIRECTION
 ******************************************************************/
enum GPIO_PIN_DIRECTION GetGpioPinDirection( const uint32_t pinnum )
{
    assert( pinnum < 54 );
    // Volatile as we're accessing register directly
    volatile uint32_t* gpioPinGroupBaseAddr = this->gpio;

    // Add controller pin group address offset GPFSELx
    switch( pinnum / 10 )
    {
    case 0: // pins 0-9
        gpioPinGroupBaseAddr += GPFSEL0;
        break;
    case 1: // pins 10-19
        gpioPinGroupBaseAddr += GPFSEL1;
        break;
    case 2: // pins 20-29
        gpioPinGroupBaseAddr += GPFSEL2;
        break;
    case 3: // pins 30-39
        gpioPinGroupBaseAddr += GPFSEL3;
        break;
    case 4: // pins 40-49
        gpioPinGroupBaseAddr += GPFSEL4;
        break;
    case 5: // pins 50-53
        gpioPinGroupBaseAddr += GPFSEL5;
        break;
    default:
        throw GPIO_EXCEPTION_GET_DIRECTION_INVALID_PIN;
    }
    // Extracts the lsb bit(0x1) for pinnum 0-9 (pinnum%10)*3
    // offset by group addr. If set, return output, else input.
    uint32_t lsb = ((*gpioPinGroupBaseAddr) & ((pinnum%10)*3) ) & 0x1;
    return (lsb) ? GPIO_PIN_DIRECTION_OUTPUT : GPIO_PIN_DIRECTION_INPUT;
}

/*******************************************************************
 * Sets the direction of a pin to either input or output
 * Pins are divided in groups of ten with the exception of pins 50-53
 * as registers denoted GPFSEL0-5. Within each register we have these
 * definitions:
 * pin 0: bits  2-0  000=input, 001=output.
 * pin 1: bits  5-3  000=input, 001=output.
 * pin 2: bits  8-6  000=input, 001=output.
 * pin 3: bits 11-9  000=input, 001=output.
 * pin 4: bits 14-12 000=input, 001=output.
 * pin 5: bits 17-15 000=input, 001=output.
 * pin 6: bits 20-18 000=input, 001=output.
 * pin 7: bits 23-21 000=input, 001=output.
 * pin 8: bits 26-24 000=input, 001=output.
 * pin 9: bits 29-27 000=input, 001=output.
 * Hence lsb = pin#*3. If lsb is set, pin is output else input.
 *
 * Parameters
 *  pinnum:       GPIO pin number as per the RPI's BCM2835's standard
 *                ranging from 0-53.
 *  direction:    Pin direction per GPIO_PIN_DIRECTION
 * Return Value:  None
 ******************************************************************/
void MmapGpio::setGpioPinDirection( const uint32_t pinnum,
                                    const enum GPIO_PIN_DIRECTION direction)
{
    assert( pinnum < 54 );
    // Volatile as we're accessing register directly
    volatile uint32_t gpioPinGroupBaseAddr = this->gpio;

    // Add controller pin group address offset GPFSELx
    switch( pinnum / 10 )
    {
    case 0: // pins 0-9
        gpioPinGroupBaseAddr += GPFSEL0;
        break;
    case 1: // pins 10-19
        gpioPinGroupBaseAddr += GPFSEL1;
        break;
    case 2: // pins 20-29
        gpioPinGroupBaseAddr += GPFSEL2;
        break;
    case 3: // pins 30-39
        gpioPinGroupBaseAddr += GPFSEL3;
        break;
    case 4: // pins 40-49
        gpioPinGroupBaseAddr += GPFSEL4;
        break;
    case 5: // pins 50-53
        gpioPinGroupBaseAddr += GPFSEL5;
        break;
    default:
        throw GPIO_EXCEPTION_SET_DIRECTION_INVALID_PIN;
    }
    // First clear the bits related to pinnum (three bits), see
    // GetGpioPinDirection for details
    (*gpioPinGroupBaseAddr) &= ~(7<<(((pinnum)%10)*3));
    // By definition 000 is input, 001 is output.
    // Hence we only have to set lsb for output then we're done.
    if ( GPIO_PIN_DIRECTION_OUTPUT == direction )
    {
        (*gpioPinGroupBaseAddr) |= (1<<(((pinnum)%10)*3));
    }
}


/*******************************************************************
 * Gets the alternate function setting of a pin
 * Pins are divided in groups of ten with the exception of pins 50-53
 * as registers denoted GPFSEL0-5. Within each register we have these
 * definitions:
 * pin 0: bits  2-0  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 1: bits  5-3  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 2: bits  8-6  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 3: bits 11-9  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 4: bits 14-12 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 5: bits 17-15 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 6: bits 20-18 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 7: bits 23-21 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 8: bits 26-24 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 9: bits 29-27 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * Hence lsb = pin#*3. Task is to return three-tuple from lsb, and
 * map it to GPIO_PIN_ALTERNATE_FUNCTION enum.
 *
 * Parameters
 *  pinnum:       GPIO pin number as per the RPI's BCM2835's standard
 *                ranging from 0-53.
 * Return Value:  Pin alternate function setting per GPIO_PIN_ALTERNATE_FUNCTION
 ******************************************************************/
enum GPIO_PIN_ALTERNATE_FUNCTION GetGpioPinAlternateFunction( const uint32_t pinnum )
{
    assert( pinnum < 54 );
    // Volatile as we're accessing register directly
    volatile uint32_t gpioPinGroupBaseAddr = this->gpio;

    // Add controller pin group address offset GPFSELx
    switch( pinnum / 10 )
    {
    case 0: // pins 0-9
        gpioPinGroupBaseAddr += GPFSEL0;
        break;
    case 1: // pins 10-19
        gpioPinGroupBaseAddr += GPFSEL1;
        break;
    case 2: // pins 20-29
        gpioPinGroupBaseAddr += GPFSEL2;
        break;
    case 3: // pins 30-39
        gpioPinGroupBaseAddr += GPFSEL3;
        break;
    case 4: // pins 40-49
        gpioPinGroupBaseAddr += GPFSEL4;
        break;
    case 5: // pins 50-53
        gpioPinGroupBaseAddr += GPFSEL5;
        break;
    default:
        throw GPIO_EXCEPTION_GET_ALTERNATE_FUNCTION_INVALID_PIN;
    }
    // Extracts the three-tuple for pinnum 0-9 (pinnum%10)*3
    // offset by group addr. Then maps to the correct alternate function.
    uint32_t threeTuple = (((*gpioPinGroupBaseAddr) & (pinnum%10)*3) & 0x7);
    GPIO_PIN_ALTERNATE_FUNCTION alternateFunction;
    switch (threeTuple)
    {
    case 0b100:
        alternateFunction = GPIO_PIN_ALTERNATE_FUNCTION_0;
        break;
    case 0b101:
        alternateFunction = GPIO_PIN_ALTERNATE_FUNCTION_1;
        break;
    case 0b110:
        alternateFunction = GPIO_PIN_ALTERNATE_FUNCTION_2;
        break;
    case 0b111:
        alternateFunction = GPIO_PIN_ALTERNATE_FUNCTION_3;
        break;
    case 0b011:
        alternateFunction = GPIO_PIN_ALTERNATE_FUNCTION_4;
        break;
    case 0b010:
        alternateFunction = GPIO_PIN_ALTERNATE_FUNCTION_5;
        break;
    default:
        throw GPIO_EXCEPTION_GET_ALTERNATE_FUNCTION_INVALID_FUNCTION;
    }
    return alternateFunction;
}

/*******************************************************************
 * Sets the alternate function setting of a pin
 * Pins are divided in groups of ten with the exception of pins 50-53
 * as registers denoted GPFSEL0-5. Within each register we have these
 * definitions:
 * pin 0: bits  2-0  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 1: bits  5-3  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 2: bits  8-6  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 3: bits 11-9  100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 4: bits 14-12 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 5: bits 17-15 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 6: bits 20-18 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 7: bits 23-21 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 8: bits 26-24 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * pin 9: bits 29-27 100=af0,101=af1,110=af2,111=af3,011=af4,010=af5
 * Hence lsb = pin#*3. Task is to locate and modify the three-tuple
 * from lsb per the .
 *
 * Parameters
 *  pinnum:       GPIO pin number as per the RPI's BCM2835's standard
 *                ranging from 0-53.
 * Return Value:  Pin alternate function setting per GPIO_PIN_ALTERNATE_FUNCTION
 ******************************************************************/
void SetGpioPinAlternateFunction( const uint32_t pinnum,
                                  const enum GPIO_PIN_ALTERNATE_FUNCTION alternateFunction )
{
    assert( pinnum < 54 );
    // Volatile as we're accessing register directly
    volatile uint32_t gpioPinGroupBaseAddr = this->gpio;

    // Add controller pin group address offset GPFSELx
    switch( pinnum / 10 )
    {
    case 0: // pins 0-9
        gpioPinGroupBaseAddr += GPFSEL0;
        break;
    case 1: // pins 10-19
        gpioPinGroupBaseAddr += GPFSEL1;
        break;
    case 2: // pins 20-29
        gpioPinGroupBaseAddr += GPFSEL2;
        break;
    case 3: // pins 30-39
        gpioPinGroupBaseAddr += GPFSEL3;
        break;
    case 4: // pins 40-49
        gpioPinGroupBaseAddr += GPFSEL4;
        break;
    case 5: // pins 50-53
        gpioPinGroupBaseAddr += GPFSEL5;
        break;
    default:
        throw GPIO_EXCEPTION_SET_ALTERNATE_FUNCTION_INVALID_PIN;
    }

    // Map the alternate function to the binary code
    uint32_t threeTuple;
    switch (alternateFunction)
    {
    case GPIO_PIN_ALTERNATE_FUNCTION_0:
        threeTuple = 0b100;
        break;
    case GPIO_PIN_ALTERNATE_FUNCTION_1:
        threeTuple = 0b101;
        break;
    case GPIO_PIN_ALTERNATE_FUNCTION_2:
        threeTuple = 0b110:
        break;
    case GPIO_PIN_ALTERNATE_FUNCTION_3:
        threeTuple = 0b111:
        break;
    case GPIO_PIN_ALTERNATE_FUNCTION_4:
        threeTuple = 0b011;
        break;
    case GPIO_PIN_ALTERNATE_FUNCTION_5:
        threeTuple = 0b010;
        break;
    default:
        throw GPIO_EXCEPTION_SET_ALTERNATE_FUNCTION_INVALID_FUNCTION;
    }
    // Clear the three-tuple for pinnum 0-9 (pinnum%10)*3
    // offset by group addr, then write new value.
    *gpioPinGroupBaseAddr &= ~(7<<((pinnum%10)*3));
    *gpioPinGroupBaseAddr |= threeTuple<<((pinnum%10)*3);
}

/*******************************************************************
 * Gets the GPIO pin level
 * GPLEVn{n=0,1} groups GPIO pins in two registers, GPLEV0{0-31} and
 * GPLEV1{32-53}.
 * Parameters
 *  pinnum:       GPIO pin number as per the RPI's BCM2835's standard
 *                ranging from 0-53.
 * Return Value:  Pin level as GPIO_PIN_LEVEL_LOW/GPIO_PIN_LEVEL_HIGH
 *****************************************************************/
enum GPIO_PIN_LEVEL GetGpioPinLevel( const uint32_t pinnum)
{
    assert( pinnum < 54 );
    // Volatile as we're accessing register directly
    volatile uint32_t gpioPinGroupBaseAddr = this->gpio;

    uint32_t value;
    if ( pinnum < 32 )
    {
        // pins 0-31
        gpioPinGroupBaseAddr += GPLEV0;
        value = (*gpioPinGroupBaseAddr) & (1<<pinnum);
    }
    else if ( pinnum < 53 )
    {
        // pins 32-53
        // Note: GPLEV0 and GPLEV1 are consecutive in memory, so we
        // could simply handle the whole thing without separating the GPLEVx
        // addresses. But for clarity this is part will be kept.
        gpioPinGroupBaseAddr += GPLEV1;
        value = (*gpioPinGroupBaseAddr) & (1<<(pinnum-32));
    }
    else
    {
        throw GPIO_EXCEPTION_SET_ALTERNATE_FUNCTION_INVALID_PIN;
    }
    return (value) ? GPIO_PIN_LEVEL_HIGH : GPIO_PIN_LEVEL_LOW;
}

/*******************************************************************
 * Sets the GPIO pin level
 * GPSETn/GPCLRn{n=0,1} groups GPIO pins in two registers, GPSET0/GPCLR0{0-31} and
 * GPSET1/GPCLR1{32-53}.
 * Note: If speed matters use inline versions - much faster!
 * If memory footprint matters, use this one.
 * Parameters
 *  pinnum:       GPIO pin number as per the RPI's BCM2835's standard
 *                ranging from 0-53.
 *  level:        GPIO_PIN_LEVEL
 * Return Value:  None
 * ****************************************************************/
void SetGpioPinLevel( const uint32_t pinnum, const enum GPIO_PIN_LEVEL level)
{
    assert( pinnum < 54 );
    // Volatile as we're accessing register directly
    volatile uint32_t gpioPinGroupBaseAddr = this->gpio;

    if ( pinnum < 32 )
    {
        // pins 0-31
        if ( GPIO_PIN_LEVEL_HIGH == level )
        {
            gpioPinGroupBaseAddr += GPSET0;
        }
        else
        {
            gpioPinGroupBaseAddr += GPCLR0;
        }
        (*gpioPinGroupBaseAddr) |= (1<<pinnum);
    }
    else if ( pinnum < 53 )
    {
        // pins 32-53
        // Note: GPSET0/GPCLR0 and GPSET1/GPCLR1 are consecutive in memory, so we
        // could simply handle the whole thing without separating the GPSETn/GPCLRn
        // addresses. But for clarity this is part will be kept.
        if ( GPIO_PIN_LEVEL_HIGH == level )
        {
            gpioPinGroupBaseAddr += GPSET1;
        }
        else
        {
            gpioPinGroupBaseAddr += GPCLR1;
        }
        (*gpioPinGroupBaseAddr) |= (1<<(pinnum-32);
    }
    else
    {
        throw GPIO_EXCEPTION_SET_ALTERNATE_FUNCTION_INVALID_PIN;
    }
}


---implementation hertil...

/*******************************************************************
 * readGpioPin() - reads the state of a GPIO pin and returns its value
 *
 * Parameter - pinnum - the pin number of the GPIO to read
 *
 * return Value - pin value. Either 1 (mmapGpio::HIGH) if pin state
 *                is high or 0 (mmapGpio::LOW) if pin is low
 * ****************************************************************/
unsigned int MmapGpio::readGpioPin(const unsigned int pinnum)
{
    assert(pinnum<54);
    if (pinnum > 53) throw GPIO_EXCEPTION_READ_INVALID_PIN;

    unsigned int retVal = 0;
    if ((*(this->gpio + GPFLEV0) & (1 << pinnum)) != 0)
        retVal = 1;

    return retVal;
}

/*******************************************************************
 * writeGpioPin() - sets (to 1) or clears (to 0) the state of an
 * output GPIO. This function has no effect on input GPIOs.
 * For faster output GPIO pin setting/clearing..use inline functions
 * 'writeGpioPinHigh()' & 'writeGpioPinLow()' defined in the header file
 *
 * Parameters - pinnum - GPIO number as per RPI and BCM2835
 *                       standard definition
 *              pinstate - value to write to output pin...either
 *              mmapGpio::HIGH for setting or mmapGpio::LOW for
 *              clearing
 * Return Value - None
 * ****************************************************************/
void MmapGpio::writeGpioPin(const unsigned int pinnum, const unsigned enum GPIO_PIN_LEVEL level)
{
    assert(pinnum<54);
    if (pinnum > 53) throw GPIO_EXCEPTION_WRITE_INVALID_PIN;
    if(level == HIGH)
        *(this->gpio + GPFSET0) = (1 << pinnum) ;
    else
        *(this->gpio + GPFCLR0) = (1 << pinnum);
}
/********************************************************************
 *	volatile unsigned *mmapGpio::mapRegAddr(unsigned long baseAddr)
 * This function maps a block of physical memory into the memory of
 * the calling process. It enables a user space process to access
 * registers in physical memory directly without having to interact
 * with in kernel side code i.e. device drivers
 *
 * Parameter - baseAddr (unsigned long) - this is the base address of
 * a block of physical memory that will be mapped into the userspace
 * process memory.
 *******************************************************************/
volatile unsigned *mmapGpio::mapRegAddr(unsigned long baseAddr)
{
    int mem_fd = 0;
    void *regAddrMap = MAP_FAILED;

    /* open /dev/mem.....need to run program as root i.e. use sudo or su */
    if (!mem_fd)
    {
        if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
        {
            perror("can't open /dev/mem");
            exit (1);
        }
    }

    /* mmap IO */
    regAddrMap = mmap(
                     NULL,             //Any adddress in our space will do
                     GPIO_LEN,       //Map length
                     PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
                     MAP_SHARED|MAP_LOCKED,       //Shared with other processes
                     mem_fd,           //File to map
                     baseAddr         //Offset to base address
                 );

    if (regAddrMap == MAP_FAILED)
    {
        perror("mmap error");
        close(mem_fd);
        exit (1);
    }

    if(close(mem_fd) < 0)  //No need to keep mem_fd open after mmap
    {
        //i.e. we can close /dev/mem
        perror("couldn't close /dev/mem file descriptor");
        exit(1);
    }
    return (volatile unsigned *)regAddrMap;
}











