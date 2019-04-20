#!/bin/sh

G_DRIVER="cpuid-g-driver.ko"
CPUID_G="cpuid-g"

[ -e $G_DRIVER ] || { echo "$G_DRIVER does not exist" ; exit 1 ; }
[ -e $CPUID_G ] || { echo "$CPUID_G does not exist" ; exit 1 ; }

SUDO=""
if [ ! -z `id -u` ]; then
   SUDO="sudo"
fi

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
