Device		:= pcd_device_setup
Driver		:= pcd_platform_driver
obj-m 		:= $(Device).o $(Driver).o
PWD 		:= $(CURDIR)
ARCH		:= arm
CROSS_COMPILE	:= arm-linux-gnueabihf-
BBB_SOURCE	:= /home/khienh/workspace/ldd/source/linux_bbb_4.14/

USER_CC		:= gcc
USER_C_FLAGS	:= -pthread

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
BBB:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(BBB_SOURCE) M=$(PWD) modules
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(BBB_SOURCE) M=$(PWD) help

tag:
	ctags -R
	cscope -Rbqk
load_device:
	sudo insmod $(Device).ko

load_driver:
	sudo insmod $(Driver).ko

unload_device:
	sudo rmmod $(Device).ko

unload_driver:
	sudo rmmod $(Driver).ko

test: test.c
	$(USER_CC) $(USER_C_FLAGS) test.c -o test
