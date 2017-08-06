#
#  Makefile for network client/server examples
#
#   make server     -- build net server for workstation
#   make server SERVER=REMOTE -- build server for target
#   make client     -- build net client for workstation

#   make netthermo  -- build thermostat server for workstation
#   make netthermo SERVER=REMOTE  -- build thermostat server for target
#   make web		-- build thermostat web server for workstation
#   make web SERVER=REMOTE	-- build thermostat web server for target

#   make maildialog -- build interactive email client for workstation
#   make popdialog -- build interactive send mail client for workstation
#   make mailclient -- build the sendmail() test application for the workstation 

CFLAGS = -g -O0 -Wall -DPORT=4201
CC_ARM := arm-linux-gcc
INC_ARM := ../../include

ifeq ($(SERVER), REMOTE)
CFLAGS += -DSERVER=\"192.168.1.50\"
netthermo: thermostat_t
web: webthermo_t

# compile everything for the ARM
netserve.o: netserve.c driver.h
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) netserve.c
netserve: netserve.o
	$(CC_ARM) -o $@ $^
thermostat.o: thermostat.c driver.h thermostat.h		
	$(CC_ARM) $(CFLAGS) -c -I$(INC_ARM) thermostat.c
monitor.o : monitor.c thermostat.h
	$(CC_ARM) $(CFLAGS) -c  -I$(INC_ARM) monitor.c
webserve.o: webserve.c webvars.h
	$(CC_ARM) $(CFLAGS) -c  -I$(INC_ARM) webserve.c
webvars.o: webvars.c webvars.h
	$(CC_ARM) $(CFLAGS) -c  -I$(INC_ARM) webvars.c
    
else
CFLAGS += -DSERVER=\"127.0.0.1\"
netthermo: thermostat_s
web: webthermo_s

# Use default compiler
thermostat.o: driver.h thermostat.h
monitor.o: thermostat.h
webserve.o: webvars.h
webvars.o: webvars.h thermostat.h
// netserve.o:
netserve: netserve.o
endif

client: netclient
netclient: netclient.o
netclient.o: netclient.c driver.h

server: netserve

# Simulation and target versions of the networked and web thermostats.
# Use the driver code in the measure directory and assume it's compiled.

thermostat_s: thermostat.o monitor.o ../measure/simdrive.o
	$(CC) -o $@ $^ -lpthread

thermostat_t: thermostat.o monitor.o ../posix/trgdrive.o
	$(CC_ARM) -o $@ $^ -lpthread

webthermo_s: thermostat.o webserve.o webvars.o ../measure/simdrive.o
	$(CC) -o $@ $^ -lpthread

webthermo_t: thermostat.o webserve.o webvars.o ../posix/trgdrive.o
	$(CC_ARM) -o $@ $^ -lpthread

# sendmail() test application

mailclient: mailclient.o sendmail.o
	$(CC) -o $@ $^

clean:
	rm -f *.o *_t *_s *~ core
