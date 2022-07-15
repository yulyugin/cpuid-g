obj-m += cpuid-g-driver.o

# BSD make does not understand "$(shell uname)"
OS!=uname|tr A-Z a-z

all: cpuid-g driver

cpuid-g: cpuid-g.c
	${CC} -Werror cpuid-g.c -o cpuid-g

.if ${OS} == linux
driver: cpuid-g-driver.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
.elif ${OS} == freebsd
driver: cpuid.c
	make -f cpuid-g-freebsd.mk
.endif

clean:
.if ${OS} == linux
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
.elif ${OS} == freebsd
	make -f cpuid-g-freebsd.mk clean
.endif
	rm -rf cpuid-g
