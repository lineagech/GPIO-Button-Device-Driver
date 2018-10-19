KDIR="/opt/linux-source-2.6.38"
PWD=$(shell pwd)

#obj-m := test_select.o
obj-m := tiny4412_button_driver.o

all:
    $(MAKE) -C ${KDIR} M=${PWD} modules
clean:
    $(MAKE) -C ${KDIR} M=${PWD} clean
