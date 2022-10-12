# cpuid-g

A set of CPU identification tools for Intel® IA-32, IA-64 and ARM® systems.

arm/  : To build for ARM®, use a C compiler capable to generate ARM® binaries. Since identification registers is a privileged resource on this architecture, a Linux kernel driver has to be loaded. This requires root privileges.
To measure CPUID value on ARM® follow the next steps:

    $ make
    $ ./cpuid-g.sh

ia32/ : To build for Intel® IA-32 a.k.a. x86/x86_64, use a C compiler to generate Intel® IA-32 binaries.

ia64/ : To build for Intel® IA-64 a.k.a. Intel® Itanium®, use a C++ compiler that is able to generate Itanium binaries.

## Results

### Intel® 64 and IA-32 CPUs

1. [Intel® Quark™ SoC X1000](dumps/ia32/Intel(R)%20Quark(TM)%20SoC%20X1000.md)
2. [Intel® Atom™ Z2560](dumps/ia32/Intel(R)%20Atom(TM)%20Z2560.md) (formerly Saltwell)
3. [Intel® Xeon Phi™ 7290](dumps/ia32/Intel(R)%20Xeon%20Phi(TM)%207290.md) (formerly Knigts Landing)
4. Intel® Core™ and Xeon® CPUs:

   | Codename     | Client variant                                                                    | Server variant
   | ---          | ---                                                                               | ---
   | Merom        | [Intel® Core™2 T7400]                                                             |
   | Penryn       | [Intel® Core™2 Duo P9500]<br>[Intel® Core™2 Duo T9600]                            |
   | Westmere     |                                                                                   | [Intel® Xeon® X5690]
   | Sandy Bridge | [Intel® Core™ i7-2600]<br>[Intel® Core™ i7-2760QM]                                | [Intel® Xeon® E5-2680]
   | Ivy Bridge   | [Intel® Core™ i7-3770]                                                            | [Intel® Xeon® E5-2680 v2]
   | Haswell      | [Intel® Core™ i5-4200U]<br>[Intel® Xeon® E3-1241 v3]                              | [Intel® Xeon® E5-2680 v3]
   | Broadwell    | [Intel® Core™ i5-5300U]                                                           | [Intel® Xeon® E5-2680 v4]<br>[Intel® Xeon® E5-2697A v4]<br>[Intel® Xeon® E5-2699 v4]
   | Skylake      | [Intel® Core™ i7-6700K]<br>[Intel® Core™ i9-7900X]<br>[Intel® Core™ i9-9960X]     | [Intel® Xeon® Gold 6140]<br>[Intel® Xeon® Gold 6142M]
   | Kaby Lake    | [Intel® Core™ i7-7700U]<br>[Intel® Core™ i7-7700K]<br>[Intel® Xeon® E3-1505M v6]<br>[Intel® Core™ i7-7567U] |
   | Coffee Lake  | [Intel® Core™ i7-8559U]<br>[Intel® Core™ i7-8700K]<br>[Intel® Core™ i7-9700K]     |
   | Cascade Lake |                                                                                   | [Intel® Xeon® Gold 6244]<br>[Intel® Xeon® Gold 6248R]<br>[Intel® Xeon® Gold 6252N]

[Intel® Core™2 T7400]: dumps/ia32/Intel(R)%20Core(TM)2%20T7400.md
[Intel® Core™2 Duo P9500]: dumps/ia32/Intel(R)%20Core(TM)2%20Duo%20P9500.md
[Intel® Core™2 Duo T9600]: dumps/ia32/Intel(R)%20Core(TM)2%20Duo%20T9600.md
[Intel® Xeon® X5690]: dumps/ia32/Intel(R)%20Xeon(R)%20X5690.md
[Intel® Core™ i7-2600]: dumps/ia32/Intel(R)%20Core(TM)%20i7-2600.md
[Intel® Core™ i7-2760QM]: dumps/ia32/Intel(R)%20Core(TM)%20i7-2760QM.md
[Intel® Xeon® E5-2680]: dumps/ia32/Intel(R)%20Xeon(R)%20E5-2680.md
[Intel® Core™ i7-3770]: dumps/ia32/Intel(R)%20Core(TM)%20i7-3770.md
[Intel® Xeon® E5-2680 v2]: dumps/ia32/Intel(R)%20Xeon(R)%20E5-2680%20v2.md
[Intel® Core™ i5-4200U]: dumps/ia32/Intel(R)%20Core(TM)%20i5-4200U.md
[Intel® Xeon® E3-1241 v3]: dumps/ia32/Intel(R)%20Xeon(R)%20E3-1241%20v3.md
[Intel® Xeon® E5-2680 v3]: dumps/ia32/Intel(R)%20Xeon(R)%20E5-2680%20v3.md
[Intel® Core™ i5-5300U]: dumps/ia32/Intel(R)%20Core(TM)%20i5-5300U.md
[Intel® Xeon® E5-2680 v4]: dumps/ia32/Intel(R)%20Xeon(R)%20E5-2680%20v4.md
[Intel® Xeon® E5-2699 v4]: dumps/ia32/Intel(R)%20Xeon(R)%20E5-2699%20v4.md
[Intel® Xeon® E5-2697A v4]: dumps/ia32/Intel(R)%20Xeon(R)%20E5-2697A%20v4.md
[Intel® Core™ i7-6700K]: dumps/ia32/Intel(R)%20Core(TM)%20i7-6700K.md
[Intel® Xeon® Gold 6140]: dumps/ia32/Intel(R)%20Xeon(R)%20Gold%206140.md
[Intel® Xeon® Gold 6142M]: dumps/ia32/Intel(R)%20Xeon(R)%20Gold%206142M.md
[Intel® Core™ i7-7700U]: dumps/ia32/Intel(R)%20Core(TM)%20i7-7700U.md
[Intel® Core™ i7-7700K]: dumps/ia32/Intel(R)%20Core(TM)%20i7-7700K.md
[Intel® Core™ i7-7567U]: dumps/ia32/Intel(R)%20Core(TM)%20i7-7567U.md
[Intel® Xeon® E3-1505M v6]: dumps/ia32/Intel(R)%20Xeon(R)%20E3-1505M%20v6.md
[Intel® Core™ i7-8559U]: dumps/ia32/Intel(R)%20Core(TM)%20i7-8559U.md
[Intel® Core™ i7-8700K]: dumps/ia32/Intel(R)%20Core(TM)%20i7-8700K.md
[Intel® Core™ i9-7900X]: dumps/ia32/Intel(R)%20Core(TM)%20i9-7900X.md
[Intel® Core™ i9-9960X]: dumps/ia32/Intel(R)%20Core(TM)%20i9-9960X.md
[Intel® Core™ i7-9700K]: dumps/ia32/Intel(R)%20Core(TM)%20i7-9700K.md
[Intel® Xeon® Gold 6244]: dumps/ia32/Intel(R)%20Xeon(R)%20Gold%206244.md
[Intel® Xeon® Gold 6248R]: dumps/ia32/Intel(R)%20Xeon(R)%20Gold%206248R.md
[Intel® Xeon® Gold 6252N]: dumps/ia32/Intel(R)%20Xeon(R)%20Gold%206252N.md

5. [AMD Ryzen™ Threadripper™ 1950X](dumps/ia32/AMD%20Ryzen%20Threadripper%201950X.md)

### Intel® IA-64 a.k.a. Intel® Itanium® CPUs

1. [Intel® Itanium® 9100](dumps/ia64/Intel(R)%20Itanium(R)%209100.md) (formerly Montvale)

### ARM® CPUs

1. AArch32:

   | Architecture   | Chip
   | ---            | ---
   | ARM1176JZFS    | [BCM2835](dumps/arm/BCM2835.md)
   | ARM Cortex A53 | [BCM2837](dumps/arm/BCM2837.md)

2. AArch64

   | Architecture   | Chip
   | ---            | ---
   | ARM Cortex A53 | [BCM2837](dumps/arm/BCM2837%20(AArch64).md)
   | ARM Cortex A72 | [BCM2711](dumps/arm/BCM2711.md)
