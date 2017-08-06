#ifndef MMAPGPIO_H
#define MMAPGPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/***********************************************************************
 * Author: Torben Frandsen
 *
 * The author is not liable for any kind of damage caused by this software.
 *
 * Acknowledgements: This 'C++' class is based on:
 * - Hussam Al-Hertani (Hertaville.com) mmapGpio class which is based on:
 * - source code from http://elinux.org/RPi_Low-level_peripherals
 * - source code from http://www.raspberrypi.org/forums/viewtopic.php?f=42&t=75048
 * - Gertboard's C source code
 *
 *
 * The MmapGpio Class is able to fully control all GPIO pins on the Raspberry Pi and
 * Raspberry Pi 2 40-pin header using 'memory map into /dev/mem' approach.
 * The advantage of this approach is that GPIO toggling speeds of tens of MHz can be
 * achieved whereas the toggling gpio speed using the sysfs interface is in the 100s
 * of KHz.
 *
 * In general pinnum range is [0-53] - if it exceeds this range an exception from
 * GPIO_EXCEPTION_CODES is thrown.
 *
 * Interrupts are triggered based on Event Detect Status register.
 * *********************************************************************/

class MmapGpio{

public:
    enum {
        GPIO_PIN_DIRECTION_INPUT = 0,
        GPIO_PIN_DIRECTION_OUTPUT
    } GPIO_PIN_DIRECTION;

    enum {
        GPIO_PIN_LEVEL_LOW = 0,
        GPIO_PIN_LEVEL_HIGH
    } GPIO_PIN_LEVEL;

    enum {
        GPIO_PIN_ALTERNATE_FUNCTION_0,
        GPIO_PIN_ALTERNATE_FUNCTION_1,
        GPIO_PIN_ALTERNATE_FUNCTION_2,
        GPIO_PIN_ALTERNATE_FUNCTION_3,
        GPIO_PIN_ALTERNATE_FUNCTION_4,
        GPIO_PIN_ALTERNATE_FUNCTION_5
    } GPIO_PIN_ALTERNATE_FUNCTION;

    enum {
        GPIO_EXCEPTION_MEMORY_UNMAP_FAILED = 0x80000001,
        GPIO_EXCEPTION_GET_DIRECTION_INVALID_PIN,
        GPIO_EXCEPTION_SET_DIRECTION_INVALID_PIN,
        GPIO_EXCEPTION_GET_ALTERNATE_FUNCTION_INVALID_PIN,
        GPIO_EXCEPTION_GET_ALTERNATE_FUNCTION_INVALID_FUNCTION,
        GPIO_EXCEPTION_SET_ALTERNATE_FUNCTION_INVALID_PIN,
        GPIO_EXCEPTION_SET_ALTERNATE_FUNCTION_INVALID_FUNCTION,
    } GPIO_EXCEPTION_CODES;

    MmapGpio();// default constructor. Mmaps into /dev/mem'
    ~MmapGpio();// removes mapping between process memory
                // & physical memory
    uint32_t GetGpioControllerBaseAddress();

    enum GPIO_PIN_DIRECTION GetGpioPinDirection( const uint32_t pinnum );
    void SetGpioPinDirection( const uint32_t pinnum, const enum GPIO_PIN_DIRECTION);

    enum GPIO_PIN_ALTERNATE_FUNCTION GetGpioPinAlternateFunction( const uint32_t pinnum );
    void SetGpioPinAlternateFunction( const uint32_t pinnum, const enum GPIO_PIN_ALTERNATE_FUNCTION);

    // Note: If speed matters use inline versions - much faster!
    // If memory footprint matters, use this one.
    void SetGpioPinLevel( const uint32_t pinnum, const enum GPIO_PIN_LEVEL );
    // Note: This optimized implementation assumes GPSET0,GPSET1 and GPCLR0,GPCLR1 are
    // consecutive in memory - which for RPi currently is correct.
    // I.e. GPSET1 = GPSET0 + 32bit, GPCLR1 = GPCLR0 + 32bit.
    // Also keep in mind there's no protection for overwriting the registers with this optimized version,
    // hence it's kept as protected - thus requires inheritance to be accessible.
protected:
    void inline SetGpioPinLevelHigh( const uint32_t pinnum){ *(gpio+GPSET0) = (1 << pinnum); }
    void inline SetGpioPinLevelLow( const uint32_t pinnum) { *(gpio+GPCLR0) = (1 << pinnum); }

public:
    enum GPIO_PIN_LEVEL GetGpioPinLevel( const uint32_t pinnum);
---implementation hertil...
    void ConfigureGpioEventDetection( const uint32_t pinnum );
    void ClearGpioEventDetection( const uint32_t pinnum );
    void ConfigureGpioRisingEdgeDetection( const uint32_t pinnum );
    void ClearGpioRisingEdgeDetection( const uint32_t pinnum );
    void ConfigureGpioFallingEdgeDetection( const uint32_t pinnum );
    void ClearGpioFallingEdgeDetection( const uint32_t pinnum );
    void ConfigureGpioHighLevelDetection( const uint32_t pinnum );
    void ClearGpioHighLevelDetection( const uint32_t pinnum );

private:
    //gpio registers starts at 0x7E200000 (bus addr), all 32-bit registers.
    //offsets here are for physical addr relative to BCM2708_PERI_BASE+BCM2708_PERI_GPIO_OFFSET (volatile *gpio)
    const unsigned enum {
        GPFSEL0 =   0,
        GPFSEL1,
        GPFSEL2,
        GPFSEL3,
        GPFSEL4,
        GPFSEL5,
        RESERVED_0X0018,
        GPSET0,
        GPSET1,
        RESERVED_0X0024,
        GPCLR0,
        GPCLR1,
        RESERVED_0X0030,
        GPLEV0,
        GPLEV1,
        RESERVED_0X003C,
        GPEDS0,
        GPEDS1,
        RESERVED_0X0048,
        GPREN0,
        GPREN1,
        RESERVED_0X0054,
        GPFEN0,
        GPFEN1
    } GPIO_CONTROLLER_REGISTERS;

    // NOTE: Update here for correct Raspberry Pi revision:
    #define BCM2708_PERIPHERALS_BASE	                0x20000000	// RPi
    //#define BCM2708_PERIPHERALS_BASE	                0x3F000000	// RPi2
    #define BCM2708_PERIPHERALS_GPIO_CONTROLLER_OFFSET	0x200000

    const uint32_t gpioControllerBaseAddr   = (BCM2708_PERIPHERALS_BASE + BCM2708_PERIPHERALS_GPIO_CONTROLLER_OFFSET);
    // Last byte address is 0x7E2000B0 + 32bit register size
    const uint32_t gpioControllerRegisterMapSizeBytes  = 0xB4;
    // GPIO register base after mapping
    volatile uint32_t* gpioControllerRegister;
    //performs memory mapping into '/dev/mem'
    volatile uint32_t* MapRegisterAddressSpaceToMemoryMap( const uint32_t baseAddr );
};

#endif
