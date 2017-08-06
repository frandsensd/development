
#  make measure  -- build measure.c for the target
#  make          -- build the simulation version of thermostat.c
#  make TARGET=1 -- build the target version of thermostat.c
#  make devices	 -- build the simulated devices program

#  Note that the only target whose build changes with the TARGET symbol
#  is thermostat.o.  Everything else builds in the appropriate environment

CFLAGS += -Wall -g
CC_ARM := arm-linux-gcc
INC_ARM := ../../include

ifdef TARGET
all: thermostat_t
thermostat.o: thermostat.c driver.h		# compile for the ARM
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) thermostat.c
else
all: thermostat_s
thermostat.o: thermostat.c driver.h	# compile for x86
endif

thermostat_s : thermostat.o simdrive.o
	$(CC) -o $@ $^
simdrive.o : simdrive.c driver.h

devices : devices.c driver.h
	$(CC) -g -o $@ -lcurses devices.c

measure : measure.o trgdrive.o
	$(CC_ARM) -o  $@ $^
measure.o : measure.c driver.h
	$(CC_ARM) $(CFLAGS) -c  -I$(INC_ARM) measure.c
trgdrive.o : trgdrive.c driver.h
	$(CC_ARM) $(CFLAGS) -c  -I$(INC_ARM) trgdrive.c

thermostat_t : thermostat.o trgdrive.o
	$(CC_ARM) -o  $@ $^

clean :
	rm -f *.o *_t *_s *~ core
