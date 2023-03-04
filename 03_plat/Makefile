File		:= pcd_device_setup
obj-m 		:= $(File).o pcd_platform_driver.o
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
load:
	sudo insmod $(File).ko

unload:
	sudo rmmod $(File).ko

test: test.c
	$(USER_CC) $(USER_C_FLAGS) test.c -o test
