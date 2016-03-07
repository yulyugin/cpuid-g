#!/bin/sh

GGG_DRIVER_KO="ggg-driver.ko"
GGG_CPUID="ggg-cpuid"

[ -e $GGG_DRIVER_KO ] || { echo "$GGG_DRIVER_KO does not exist" ; exit 1 ; }
[ -e $GGG_CPUID ] || { echo "$GGG_CPUID does not exist" ; exit 1 ; }

SUDO=""
if [ ! -z `id -u` ]; then
   SUDO="sudo"
fi

$SUDO /sbin/insmod $GGG_DRIVER_KO

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to load $GGG_DRIVER_KO"
   exit $RET
fi

$SUDO `readlink -f $GGG_CPUID`

$SUDO /sbin/rmmod $GGG_DRIVER_KO

RET=$?
if [ $RET -ne 0 ]; then
   echo "Failed to unload $GGG_DRIVER_KO"
   exit $RET
fi
