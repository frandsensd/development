#
# 
#  Makefile for ARM/Linux user application
# 

SHELL = /bin/sh

CFLAGS += -Wall -g
CC = arm-linux-gcc

APP = devmem

all: ${APP}

${APP}: ${APP}.c
	${CC} -o ${APP} $?

clean:
	rm -f *.o
