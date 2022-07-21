#!/bin/sh

SUDO=""
if [ `id -u` != 0 ]; then
   SUDO="sudo"
fi

if [ $OSTYPE = "FreeBSD" ]; then
    SUDO=$SUDO ./freebsd/cpuid-g.sh
    exit
fi

G_DRIVER="linux/cpuid-g.ko"
CPUID_G="app/cpuid-g"

[ -e $G_DRIVER ] || { echo "$G_DRIVER does not exist" ; exit 1 ; }
[ -e $CPUID_G ] || { echo "$CPUID_G does not exist" ; exit 1 ; }

$SUDO /sbin/insmod $G_DRIVER

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to load $G_DRIVER"
   exit $RET
fi

$SUDO `readlink -f $CPUID_G`

$SUDO /sbin/rmmod $G_DRIVER

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to unload $G_DRIVER"
   exit $RET
fi
