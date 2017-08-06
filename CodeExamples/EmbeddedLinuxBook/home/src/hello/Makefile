obj-m := hello.o 

# Comment/uncomment the following line to disable/enable debugging
# DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DDEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBFLAGS)

all: modules
# To build modules outside of the kernel tree, we run "make" in the kernel
# source tree with an environment variable that points to this directory

modules:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(shell pwd)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions ?odule*
