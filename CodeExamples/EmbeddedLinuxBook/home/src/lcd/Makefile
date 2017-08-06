#
# 
#  Makefile for ARM/Linux user application
# 

CFLAGS = -g -O0 -Wall -DPORT=4201 -DSERVER=\"192.168.1.50\"
CC_ARM := arm-linux-gcc
INC_ARM := ../../include

all: lcdtest

lcdtest: lcdtest.o lcdutils.o
	${CC_ARM} -o $@ $^
    
lcdtest.o: lcdtest.c lcdutils.h
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) lcdtest.c
lcdutils.o: lcdutils.c lcdutils.h
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) lcdutils.c
thermostat.o: thermostat.c		
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) thermostat.c
monitor.o: monitor.c		
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) monitor.c

netthermo_t: thermostat.o monitor.o ../posix/trgdrive.o lcdutils.o
	$(CC_ARM) -o $@ $^ -lpthread

clean:
	rm -f *.o
