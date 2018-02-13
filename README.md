# ggg-cpuid

A set of CPU identification tools for Intel® IA-32, IA-64 and ARM systems.

arm/  : To build for ARM, use a C compiler capable to generate ARM binaries. Since identification registers is a privileged resource on this architecture, a Linux kernel driver has to be loaded. This requires root privileges.
To measure CPUID value on ARM follow the next steps:

    $ make
    $ ./ggg-cpuid.sh

ia32/ : To build for Intel® IA-32 a.k.a. x86/x86_64, use a C compiler to generate Intel® IA-32 binaries.

ia64/ : To build for Intel® IA-64 a.k.a. Intel® Itanium®, use a C++ compiler that is able to generate Itanium binaries.

## Results

### Intel® 64 and IA-32 CPUs

1. [Intel® Quark™ SoC X1000](dumps/ia32/Intel®%20Quark™%20SoC%20X1000.md)
2. [Intel® Xeon Phi™ 7290](dumps/ia32/Intel®%20Xeon%20Phi™%207290.md) (formerly Knigts Landing)
3. Intel® Core™ and Xeon® CPUs:

   | Codename  | Client variant                                                         | Server variant                                                         |
   | ---       | ---                                                                    | ---                                                                    |
   | Penryn    | [Intel® Core™2 Duo P9500](dumps/ia32/Intel®%20Core™2%20Duo%20P9500.md) |                                                                        |
   | Haswell   | [Intel® Core™ i5-4200U](dumps/ia32/Intel®%20Core™%20i5-4200U.md)       | [Intel® Xeon® E5-2680 v3](dumps/ia32/Intel®%20Xeon®%20E5-2680%20v3.md) |
   | Broadwell | [Intel® Core™ i5-5300U](dumps/ia32/Intel®%20Core™%20i5-5300U.md)       | [Intel® Xeon® E5-2680 v4](dumps/ia32/Intel®%20Xeon®%20E5-2680%20v4.md) |
   | Skylake   | [Intel® Core™ i7-6700K](dumps/ia32/Intel®%20Core™%20i7-6700K.md)       |                                                                        |
