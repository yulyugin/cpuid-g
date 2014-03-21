/* Print CPUID the right way
 *
 * Copyright 2014, Grigory Rechistov
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <string.h>
#include "optionparser.h"

struct cpuid_result {
    bool taken;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};

cpuid_result do_cpuid(uint32_t leaf, uint32_t subleaf) {
    uint32_t eax, ebx, ecx, edx;
    __asm__ __volatile__ (
        "movl $0, %%ebx \n"
        "movl $0, %%edx \n"
        "cpuid \n"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a"(leaf), "c"(subleaf)
        :
        );

    cpuid_result r = {.taken = 0};
    r.taken = eax || ebx || ecx || edx;
    r.eax = eax;
    r.ebx = ebx;
    r.ecx = ecx;
    r.edx = edx;
    return r;
}

static void cpuid_leaf(uint32_t leaf) {
    int subleaf = 0;

    cpuid_result last_subleaf;
    memset(&last_subleaf, 0, sizeof(last_subleaf));

    for (subleaf = 0; subleaf > -1; ++subleaf) {
        cpuid_result r = do_cpuid(leaf, subleaf);

        // There is no standart way to determine a count of subleaves.
        // We use assumption that there is no subleaves with the same values.
        // So if we come on the subleaf with value like in the previous subleaf
        // we deside that there is no more valid subleaves.

        switch(leaf) {
            case 0x7:
                // EAX: Reports the maximum input value for
                // supported leaf 7 sub-leaves.
                if (subleaf >= r.eax)
                    return;
            case 0xb:
                // Most of Leaf 0BH output depends on the initial value in ECX.
                // The EDX output of leaf 0BH is always valid and does not vary
                // with input value in ECX.
                // Output value in ECX[7:0] always equals input value in
                // ECX[7:0].
                // For sub-leaves that return an invalid level-type of 0 in
                // ECX[15:8]; EAX and EBX will return 0.
                // If an input value n in ECX returns the invalid level-type of
                // 0 in ECX[15:8], other input values with ECX >
                // n also return 0 in ECX[15:8].
                if ((r.eax || r.ebx || (r.ecx & ~0xff)) == 0)
                    return;
            default:
                if ((r.eax || r.ebx || r.ecx || r.edx) == 0)
                    return;

                if (!memcmp(&last_subleaf, &r, sizeof(last_subleaf)))
                    return;
        }
        printf("  %#10x  %#10x  0x%08x  0x%08x  0x%08x  0x%08x\n", leaf, subleaf, r.eax, r.ebx, r.ecx, r.edx);
        last_subleaf = r;
    }
}

static void cpuid_level(uint32_t level) {
    cpuid_result r = do_cpuid(level, 0);

    int leaf;
    for (leaf = level; leaf < r.eax; ++leaf) {
        cpuid_leaf(leaf);
    }
}

static void dump_cpuid() {
    printf("Leaf             Subleaf         EAX         EBX        ECX          EDX\n");
    printf("------------------------------------------------------------------------\n");
    cpuid_level(0);
    cpuid_level(0x80000000);
}

int main(int argc, char **argv) {
    // TODO add option checking
    enum  optionIndex { UNKNOWN, HELP, LEAF, SUBLEAF };
    const option::Descriptor usage[] =
    {
     {UNKNOWN, 0,"" , ""    ,   option::Arg::None, "USAGE: ggg-cpuid [options]\n\n"
                                                "Options:" },
     {HELP,    0,"" , "help",   option::Arg::None, "  --help  \tPrint usage and exit." },
     {LEAF,    0,"l", "leaf",   option::Arg::None, "  --leaf, -l  \tPrint just this leaf" },
     {SUBLEAF, 0,"s", "subleaf",option::Arg::None, "  --subleaf, -s  \tUse this particular subleaf" },
     
     {UNKNOWN, 0,"" ,  ""   ,   option::Arg::None, "\n\n" },
     {0,0,0,0,0,0}
    };
    argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
    option::Stats  stats(usage, argc, argv);
    option::Option options[stats.options_max], buffer[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);
    if (parse.error())
      return 1;
    
    if (options[HELP]) {
      option::printUsage(std::cout, usage);
      return 0;
    }
    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
      std::cout << "Unknown option: " << opt->name << "\n";   

    dump_cpuid();
    return 0;
}
