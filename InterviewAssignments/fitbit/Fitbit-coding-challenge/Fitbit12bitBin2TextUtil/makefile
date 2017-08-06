TARGET = ./../12bitBin2TextUtil
LIBS = -lm
TESTFILEPATH = ../dataset
TESTFILE = $(TESTFILEPATH)/test3
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

#Auto-locate all source and header files in local directory
#using patsubst and wildcard features of make utility:
#OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS = usage.o \
          fifoBuffer.o \
          list.o \
          readInputData.o \
          writeResults.o \
          Fitbit12bitBin2TextUtil.o

HEADERS = $(wildcard *.h)

#Object dependencies:
%.o: %.c (HEADERS)
	$(CC) $(CCFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

#Target dependencies:
$(TARGET): $(OBJECTS)
	$(CC) $(CLFLAGS) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f $(TESTFILEPATH)/test*.txt

#Execute:
run:
	$(TARGET) $(TESTINFILE) $(TESTOUTFILE) 
#	-diff --strip-trailing-cr $(TESTOUTFILE) $(TESTREFFILE)

