ggg-cpuid
=========

A set of CPU identification tools for Intel IA-32, IA-64 and ARM systems.

arm/  : To build for ARM, use a C compiler capable to generate ARM binaries. Since identification registers is a privileged resource on this architecture, a Linux kernel driver has to be loaded. This requires root privileges.
To measure CPUID value on ARM follow the next steps:

    $ make
    $ ./ggg-cpuid.sh

ia32/ : To build for IA-32 a.k.a. x86/x86_64, use a C compiler to generate IA-32 binaries.

ia64/ : To build for IA-64 a.k.a. Intel Itanium, use a C++ compiler that is able to generate Itanium binaries.
