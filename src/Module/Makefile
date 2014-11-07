ifneq ($(KERNELRELEASE),)
#1.
	# obj-m := terminal.o
	# terminal-objs := terminal.o shuttle.o

#2.	
	# TARGET = terminal

	# terminal-objs := shuttle.o 

	# obj-m := $(TARGET).o
#3 works eh
	#obj-m := terminal.o

#4 
	obj-m := terminal.o
	obj-m += shuttle.o
	#obj-m += passenger.o
else
	KERNELDIR ?= /lib/modules/`uname -r`/build/
	PWD := `pwd`
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif
