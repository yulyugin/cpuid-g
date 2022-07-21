#!/bin/sh

CPUID_G="app/cpuid-g"
G_DRIVER="freebsd/cpuid-g.ko"

[ -e $G_DRIVER ] || { echo "$G_DRIVER does not exist" ; exit 1 ; }
[ -e $CPUID_G ] || { echo "$CPUID_G does not exist" ; exit 1 ; }

$SUDO /sbin/kldload $G_DRIVER

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to load $G_DRIVER"
   exit $RET
fi

$SUDO `readlink -f $CPUID_G`

$SUDO /sbin/kldunload $G_DRIVER

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to unload $G_DRIVER"
   exit $RET
fi
