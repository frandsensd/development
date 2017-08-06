/******************************************************************************
 
  Filename: lcdtest.c
 
    Simple test program for the LCD utilities
    
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "lcdutils.h"

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
#define BUFLEN 20

int running = 1;
const char *delims = " \t,=";

int main (int argc, char *argv[])
{
    unsigned int row, col, width;
    int value;
    char text[BUFLEN], string[BUFLEN];
/*
    Initialization
*/
    if (LCD_init() == -1)
        FATAL;
/*
    Command loop
*/
    while (running)
    {
        printf ("LCD>");
        fgets (text, BUFLEN, stdin);

        switch (*text)
        {
            case 's':   // write string
                sscanf (text+2, "%d %d %s", &row, &col, string);
                LCD_write_string (row, col, string);
                break;
                    
            case 'n':   // write number
                sscanf (text+2, "%d %d %d %d", &row, &col, &value, &width);
                LCD_write_number (row, col, value, width);
                break;

            case 'b':   // backlight on/off
                sscanf (text+2, "%s", string);
                LCD_backlight (strcmp (string, "off"));
                break;
                    
            case 'q':   // quit program
                running = 0;
                break;
                
            default:
                continue;
        }
    }       
    LCD_close();
    return 0;
}
