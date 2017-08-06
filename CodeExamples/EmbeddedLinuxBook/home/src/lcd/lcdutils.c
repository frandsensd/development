/******************************************************************************
 
  Filename: LCDutil.c
 
    Set of simple utility functions for manipulating the E.L.L.K. LCD screen
    
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "lcdutils.h"

static int fd, fback;
static char text [20];

void LCD_erase (int what)
{
    switch (what)
    {
        case LCD_LINE:
            write (fd, "\x1b[2K", 4);
            break;
        case LCD_SCREEN:
            write (fd, "\x1b[2J", 4);
            break;
        default:
            break;
    }
}

int LCD_init (void)
{
// Use the existing backlight driver
    fback = open ("/sys/class/leds/backlight/brightness", O_RDWR | O_SYNC);
    if (fback == -1)
        return -1;

// Open the console device  
    fd = open ("/dev/tty0", O_RDWR | O_SYNC);
    if (fd == -1)
        return -1;

    LCD_erase (LCD_SCREEN);
    LCD_backlight (1);
    
    return fd;
}

void LCD_set_cursor (int row, int col)
{
    sprintf (text, "\x1b[%d;%dH", row, col);
    write (fd, text, strlen (text));
}

void LCD_write_string (int row, int col, char *string)
{
    LCD_set_cursor (row, col);
    write (fd, string, strlen (string));
}

void LCD_write_number (int row, int col, int value, int len)
{
    char text[10];
    
    sprintf (text, "%*d", len, value);
    LCD_write_string (row, col, text);
}

void LCD_backlight (int on)
{
	static const char onoff[] = {'0', '1'};
	int i = 0;

	if (on) i++;
	write (fback, &onoff[i], 1);
}

void LCD_close (void)
{
    LCD_backlight (0);
    close (fd);
    close (fback);
}
