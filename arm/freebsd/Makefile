ARCH != uname -m

.if $(ARCH) == arm64
CFLAGS += -DCONFIG_ARM64
.endif

CFLAGS+=-I../include
SRCS=cpuid-g.c
KMOD=cpuid-g

.include <bsd.kmod.mk>
