Device		:= vi2c_dev
Driver		:= vi2c_drv
iic		:= vi2c
obj-m 		:= $(Device).o $(Driver).o $(iic).o
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
	sudo insmod $(Device).ko
	sudo insmod $(Driver).ko
	sudo insmod $(iic).ko
unload:
	sudo rmmod $(Device).ko
	sudo rmmod $(Driver).ko
	sudo rmmod $(iic).ko
