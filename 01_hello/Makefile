obj-m 		:= hello.o
PWD 		:= $(CURDIR)
ARCH		:= arm
CROSS_COMPILE	:= arm-linux-gnueabihf-
BBB_SOURCE	:= /home/khienh/workspace/ldd/source/linux_bbb_4.14/


all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
BBB:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(BBB_SOURCE) M=$(PWD) modules
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(BBB_SOURCE) M=$(PWD) help
