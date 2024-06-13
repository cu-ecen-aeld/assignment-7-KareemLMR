
SUBDIRS =  misc-progs misc-modules \
           skull scull scullc scullp lddbus sculld scullv shortprint simple tty \
	   pci usb\
	   sbull snull short

modules:
	cd scull
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules
        cd ../misc-modules
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

all: subdirs

subdirs:
        cd scull
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules
        cd ../misc-modules
        $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done
