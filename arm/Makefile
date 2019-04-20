obj-m += cpuid-g-driver.o

all: cpuid-g driver

cpuid-g: cpuid-g.c
	gcc -Werror cpuid-g.c -o cpuid-g

driver: cpuid-g-driver.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf cpuid-g
