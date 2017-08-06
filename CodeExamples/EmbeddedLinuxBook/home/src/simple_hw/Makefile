obj-m := simple_hw.o simple_int.o

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Add your debugging flag (or not) to EXTRA_CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DDEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBFLAGS) -DNR_PORTS=8 -I$(HOME)/target_fs/home/include

all: modules
# To build modules outside of the kernel tree, we run "make" in the kernel
# source tree with an environment variable that points to this directory

modules:
	$(MAKE) -C /usr/src/arm/linux M=$(shell pwd)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions ?odule*
