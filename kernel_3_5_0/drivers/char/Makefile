PWD := $(shell pwd)

obj-m := tiny4412_button_driver.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	sudo cp tiny4412_button_driver.o $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko .tmp_versions modules.order Module.symvers

.PHONY: modules install clean
