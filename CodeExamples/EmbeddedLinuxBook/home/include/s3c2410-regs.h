/*
        Register definitions for Samsung S3c2410 SoC
        ARM processor

        05/23/10  Doug Abbott

        These declarations are intended to be used for accessing
        I/O registers directly from applications.  Some will argue
        that accessing I/O from application level is not good practice,
        but it makes it easy to illustrate hardware access without
        having to dive into device drivers.

        Initially, this file just defines the GP I/O port registers and
        the ADC.  As we go along, it will be expanded to include other
        needed registers.
*/

#ifndef S3C2410_REGS_H
#define S3C2410_REGS_H

typedef volatile unsigned int S3C2410_REG;// Hardware register definition

/*****************************************************************************
              SOFTWARE REGISTER DEFINITIONS FOR System Peripherals
*****************************************************************************/
typedef struct _S3C2410_GPIO {
// Port A (0:24)
    S3C2410_REG     GPACON;          // control
    S3C2410_REG     GPADAT;          // data
    S3C2410_REG     Reserved1[2];
// Port B (0:10)
    S3C2410_REG     GPBCON;          // control
    S3C2410_REG     GPBDAT;          // data
    S3C2410_REG     GPBUP;           // pull ups
    S3C2410_REG     Reserved2;
// Port C (0:15)
    S3C2410_REG     GPCCON;          // control
    S3C2410_REG     GPCDAT;          // data
    S3C2410_REG     GPCUP;           // pull ups
    S3C2410_REG     Reserved3;
// Port D (0:15)
    S3C2410_REG     GPDCON;          // control
    S3C2410_REG     GPDDAT;          // data
    S3C2410_REG     GPDUP;           // pull ups
    S3C2410_REG     Reserved4;
// Port E (0:15)
    S3C2410_REG     GPECON;          // control
    S3C2410_REG     GPEDAT;          // data
    S3C2410_REG     GPEUP;           // pull ups
    S3C2410_REG     Reserved5;
// Port F (0:7)
    S3C2410_REG     GPFCON;          // control
    S3C2410_REG     GPFDAT;          // data
    S3C2410_REG     GPFUP;           // pull ups
    S3C2410_REG     Reserved6;
// Port G (0:15)
    S3C2410_REG     GPGCON;          // control
    S3C2410_REG     GPGDAT;          // data
    S3C2410_REG     GPGUP;           // pull ups
    S3C2410_REG     Reserved7;
// Port H (0:10)
    S3C2410_REG     GPHCON;          // control
    S3C2410_REG     GPHDAT;          // data
    S3C2410_REG     GPHUP;           // pull ups
    S3C2410_REG     Reserved8;
// Port J (0:12)
    S3C2410_REG     GPJCON;          // control
    S3C2410_REG     GPJDAT;          // data
    S3C2410_REG     GPJUP;           // pull ups
    S3C2410_REG     Reserved9;
} S3C2410_GPIO, *S3C2410P_GPIO;

// Control bit patterns for GPIO pins
#define PIN 0           // pin is input
#define POUT 1          // pin is output
#define PCON 2          // pin is peripheral function

// Each data bit requires two control bits
#define GP_BIT(bit, pattern) (pattern << 2*bit)

void inline write_reg (S3C2410_REG *regp, unsigned int mask, unsigned int pattern)
{
// writes pattern to the part of regp defined by mask
    *regp = (*regp & ~mask) | pattern;
}

void inline set_reg (S3C2410_REG *regp, unsigned int pattern)
{
    *regp |= pattern;
}

void inline clear_reg (S3C2410_REG *regp, unsigned int pattern)
{
    *regp &= ~pattern;
}

typedef struct _S3C2410_ADC {
    S3C2410_REG     CON;            // control
    S3C2410_REG     TSC;            // touch screen control
    S3C2410_REG     DLY;            // delay
    S3C2410_REG     DAT0;           // X position data and normal ADC
    S3C2410_REG     DAT1;           // Y position data
    S3C2410_REG     UPDN;           // stylus up/down interrupt status
} S3C2410_ADC, *S3C2410P_ADC;

// ADC->CON bits
#define ECFLG       0x8000          // end of conversion
#define PRSCEN      0x4000          // prescaler enable
#define PRSCVL(x)   ((x&0xff) << 6) // prescale value (8 bits)
#define CHSEL(x)    ((x&7) << 3)    // channel select
#define STDBM       0x4             // standby mode
#define RD_STRT     0x2             // start conversion after read
#define START       0x1             // manual start

// ADC->TSC bits
#define UDSEN       0x100           // detect stylus up/down status
#define YMSEN       0x80            // YM switch enable
#define YPSEN       0x40            // YP switch enable
#define XMSEN       0x20            // XM switch enable
#define XPSEN       0x10            // XP switch enable
#define PULL_UP     0x8             // pull up switch enable
#define AUTO_PST    0x4             // auto sequencing conversion
#define XPOS        1               // X-position measurement
#define YPOS        2               // Y-position measurement
#define WINT        3               // wait for interrupt mode

// ADC->DAT bits
#define UPDOWN      0x8000          // stylus state
#define AUTOPST     0x4000          // auto sequencing conversion
#define XY_MAN      0x3000          // manual measurement modes

#endif      // S3C2410_REGS_H
