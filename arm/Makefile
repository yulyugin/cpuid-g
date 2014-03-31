obj-m += ggg-driver.o

all: ggg-cpuid driver

ggg-cpuid: ggg-cpuid.c
	gcc -Werror ggg-cpuid.c -o ggg-cpuid

driver: ggg-driver.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf ggg-cpuid
