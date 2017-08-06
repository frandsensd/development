/*
 * devmem2.c: Simple program to read/write from/to any location in memory.
 *
 *  Copyright (C) 2000, Jan-Derk Bakker (jdb@lartmaker.nl)
 *
 *
 * This software has been developed for the LART computing board
 * (http://www.lart.tudelft.nl/). The development has been sponsored by
 * the Mobile MultiMedia Communications (http://www.mmc.tudelft.nl/)
 * and Ubiquitous Communications (http://www.ubicom.tudelft.nl/)
 * projects.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Modified by Doug Abbott
 * This version maps a page of memory then runs in a loop accepting read and write
 * commands to that page.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
  
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
#define BUFLEN 80

const char *delims = " \t,=";

int main (int argc, char **argv)
{
    int fd;
    void *map_base, *virt_addr; 
	char text[BUFLEN], *cmd;
	unsigned int target, read_result, writeval;
	int access_type = 'h';
	
	if(argc < 2)
	{
		printf ("Page to map: ");
		fgets (text, BUFLEN, stdin);
		target = strtoul (text, 0, 16);
	}
	else
		target = strtoul (argv[1], 0, 16);

    fd = open ("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1)
        FATAL;
    printf ("/dev/mem opened.\n"); 
    fflush (stdout);
    
    /* Map one page */
    target &=  ~MAP_MASK;
    map_base = mmap (0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target);
    if (map_base == (void *) -1) FATAL;
    printf ("Memory %x mapped at address %p.\n", target, map_base);
    printf ("Initial access mode: half word (16 bits)\n");
    fflush (stdout);
    
    do
    {
    	printf ("devmem> ");
        fgets (text, BUFLEN, stdin);
    	cmd = strtok (text, delims);
    
    	switch (cmd[0])
    	{
			case 'b':   // byte access
			case 'h':   // "half" word, 16-bit access
			case 'l':   // "long" word, 32-bit access
				access_type = cmd[0];
				printf ("Access type %c selected\n", access_type);
				break;
			
			case 'r':   // read a location
				target = strtoul (strtok (NULL, delims), 0, 16);
    			virt_addr = map_base + (target & MAP_MASK);
				switch (access_type)
				{
					case 'b':
						read_result = *((unsigned char *) virt_addr);
						break;
					case 'h':
						read_result = *((unsigned short *) virt_addr);
						break;
					case 'l':
						read_result = *((unsigned long *) virt_addr);
						break;
				}
    			printf ("0x%X (%p) = 0x%X\n", target, virt_addr, read_result); 
    			fflush (stdout);
    			break;

			case 'w':   // write a location
				target = strtoul (strtok (NULL, delims), 0, 16);
				writeval = strtoul (strtok (NULL, delims), 0, 16);
    			virt_addr = map_base + (target & MAP_MASK);
				switch (access_type)
				{
					case 'b':
						*((unsigned char *) virt_addr) = writeval;
						read_result = *((unsigned char *) virt_addr);
						break;
					case 'h':
						*((unsigned short *) virt_addr) = writeval;
						read_result = *((unsigned short *) virt_addr);
						break;
					case 'l':
						*((unsigned long *) virt_addr) = writeval;
						read_result = *((unsigned long *) virt_addr);
						break;
				}
				printf ("Wrote 0x%X to 0x%X (%p); readback 0x%X\n", writeval, target, virt_addr, read_result); 
				fflush (stdout);
				break;
				
			case 'q':	// this is the end, nothing to do
				break;
				
			default:
				printf ("Invalid command %s\n", cmd);
    	}
	}
   	while (cmd[0] != 'q');
	
	if (munmap (map_base, MAP_SIZE) == -1) FATAL;
    close (fd);
    return 0;
}

