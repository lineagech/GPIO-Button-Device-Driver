KDIR="/opt/linux-source-2.6.38"
PWD=$(shell pwd)

obj-m := test_select.o

all:
    $(MAKE) -C ${KDIR} M=${PWD} modules
clean:
    $(MAKE) -C ${KDIR} M=${PWD} clean
