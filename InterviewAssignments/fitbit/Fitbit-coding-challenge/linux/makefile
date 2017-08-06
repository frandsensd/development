TARGET = ./12bitBin2TextUtil
LIBS = 
VPATH = ../source
OBJECTPATH = obj

TESTFILEPATH = ../dataset
TESTFILE = $(TESTFILEPATH)/test5
TESTINFILE  = $(TESTFILE).bin
TESTOUTFILE = $(TESTFILE).txt
TESTREFFILE = $(TESTFILE).out

#Compiler:
CC = gcc

#Compiler flags:
CCFLAGS = -c -ansi -std=c99 -Wall -g

#Linker:
CL = gcc
CLFLAGS = -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(addprefix $(OBJECTPATH)/, \
			usage.o \
			fifoBuffer.o \
			list.o \
			readInputData.o \
			writeResults.o \
			Fitbit12bitBin2TextUtil.o )

HEADERS = $(wildcard *.h)

#Object dependencies:
$(OBJECTPATH)/%.o: %.c
	$(CC) $(CCFLAGS) $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

#Target dependencies:
$(TARGET): $(OBJECTS)
	$(CC) $(CLFLAGS) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f $(TESTFILEPATH)/test*.txt

#Execute:
run:
	$(TARGET) $(TESTINFILE) $(TESTOUTFILE) 

