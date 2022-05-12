ARCH != uname -m

.if $(ARCH) == arm64
CFLAGS += -DCONFIG_ARM64
.endif

SRCS=cpuid-g.c
KMOD=cpuid-g

.include <bsd.kmod.mk>
