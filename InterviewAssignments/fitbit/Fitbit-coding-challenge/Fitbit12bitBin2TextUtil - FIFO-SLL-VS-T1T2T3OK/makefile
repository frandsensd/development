TARGET = ./../12bitBin2TextUtil
LIBS = -lm
TESTINFILE = ../dataset/test1.bin
TESTOUTFILE = ../dataset/test1.txt

#Compiler:
CC = gcc
CCFLAGS = -ansi -Wall
#Add -g for debug info
#Add -std=c99 for C99 specs

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
	-rm -f $(TESTOUTFILE)

#Execute:
run:	$(TARGET)
	$(TARGET) $(TESTINFILE) $(TESTOUTFILE) 
	-cat $(TESTOUTFILE) | more
