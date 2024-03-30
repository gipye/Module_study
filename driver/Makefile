obj-m += virtual_device.o

KDIR := /lib/modules/$(shell uname -r)/build

default:
	make -C $(KDIR) M=$(PWD) modules
	gcc a.c
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm a.out
