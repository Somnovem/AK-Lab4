# Check whether we are being invoked from the kernel build system
ifneq ($(KERNELRELEASE),)

# --- Kernel module build context ---
obj-m := hello.o

else

# --- User-invoked build context ---
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif