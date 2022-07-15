#!/bin/sh

OS=$(uname)
G_DRIVER="cpuid-g-driver.ko"
CPUID_G="cpuid-g"

[ -e $G_DRIVER ] || { echo "$G_DRIVER does not exist" ; exit 1 ; }
[ -e $CPUID_G ] || { echo "$CPUID_G does not exist" ; exit 1 ; }

SUDO=""
if [ "$OS" = "FreeBSD" ] ; then
    if [ `id -u` -ne 0 ]; then
	echo "need sudo"
	SUDO="sudo"
    fi
else
    if [ ! -z `id -u` ]; then
	echo "need sudo"
	SUDO="sudo"
    fi
fi

if [ "$OS" = "FreeBSD" ] ; then
    $SUDO /sbin/kldload ./$G_DRIVER
else
    $SUDO /sbin/insmod $G_DRIVER
fi

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to load $G_DRIVER"
   exit $RET
fi

$SUDO `readlink -f $CPUID_G`

if [ "$OS" = "FreeBSD" ] ; then
    $SUDO /sbin/kldunload $G_DRIVER
else
    $SUDO /sbin/rmmod $G_DRIVER
fi

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to unload $G_DRIVER"
   exit $RET
fi
