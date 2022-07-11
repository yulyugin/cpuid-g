obj-m += cpuid-g.o

SRCBASE ?= $(PWD)/..

EXTRA_CFLAGS += -I$(SRCBASE)/include

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(SRCBASE)/linux modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(SRCBASE)/linux clean

