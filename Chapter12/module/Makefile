KDIR = /lib/modules/`uname -r`/build
obj-m := hello_module.o \
         gpio_module.o \
         gpiofunction_module.o \
         gpioirq_module.o \
	 gpiotimer_module.o \
         gpiosignal_module.o

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
