/* Print CPUID the right way
 *
 * Copyright (c) 2014, Evgeny Yulyugin and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * The names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpuid-g.h"

typedef struct cpuid_result_t {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpuid_result;

static cpuid_result do_cpuid(uint32_t leaf, uint32_t subleaf) {
    uint32_t eax, ebx, ecx, edx;
    __asm__ __volatile__ (
        "movl $0, %%ebx \n"
        "movl $0, %%edx \n"
        "cpuid \n"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a"(leaf), "c"(subleaf)
        );

    cpuid_result r;
    r.eax = eax;
    r.ebx = ebx;
    r.ecx = ecx;
    r.edx = edx;
    return r;
}

static void print_subleaf(uint32_t leaf, uint32_t subleaf, cpuid_result r) {
    /* 10 bytes for string and one for \0. snprintf will automatically add it */
    char _leaf[11], _eax[11], _ebx[11], _ecx[11], _edx[11];
    snprintf(_leaf, 11, "0x%x", leaf);
    snprintf(_eax, 11, "0x%x", r.eax);
    snprintf(_ebx, 11, "0x%x", r.ebx);
    snprintf(_ecx, 11, "0x%x", r.ecx);
    snprintf(_edx, 11, "0x%x", r.edx);
    printf("%10s  %10d  %10s  %10s  %10s  %10s\n",
           _leaf, subleaf, _eax, _ebx, _ecx, _edx);
}

static int msb64(uint64_t v)
{
    int n = 0;
    int count = 32;
    while (count) {
        if ((v & ~((1ull << count) - 1)) != 0) {
            n += count;
            v >>= count;
        }
        count /= 2;
    }
    return n;
}

void cpuid_leaf(uint32_t leaf) {
    uint32_t subleaf = 0;
    uint32_t max_subleaf = 0xffffffff - 1; /* -1 to avoid infinit loops */

    /* Some leaves specify mask of valid subleaves if leaf 0.
     * Subleaf 0 is always valid. */
    uint64_t subleaf_mask = 1;

    cpuid_result last_subleaf;
    memset(&last_subleaf, 0, sizeof(last_subleaf));

    for (subleaf = 0; subleaf <= max_subleaf; subleaf++) {
        cpuid_result r = do_cpuid(leaf, subleaf);

        // There is no standart way to determine a count of subleaves.
        // We use assumption that there is no subleaves with the same values.
        // So if we come on the subleaf with value like in the previous subleaf
        // we deside that there is no more valid subleaves.

        switch (leaf) {
            case 0x1:
                // Clear "Initial APIC ID" field.
                // Always pretend that we run on CPU 0.
                r.ebx &= 0xffffff;
                break;

            case 0x7:
            case 0x14:
            case 0x17:
                // Subleaf 0 of several leaves reports the maximum
                // input value for supported sub-leaves in EAX.
                if (subleaf == 0)
                    max_subleaf = r.eax;
                break;

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
                break;

            case 0xd:
                // Each sub-leaf index (starting at position 2) is supported
                // if it corresponds to a supported bit in either the
                // XCR0 register or the IA32_XSS MSR.
                if (subleaf == 0) {
                    subleaf_mask |= (uint64_t)r.edx << 32 | r.eax;
                } else if (subleaf == 1) {
                    subleaf_mask |= (uint64_t)r.edx << 32 | r.eax;
                    max_subleaf = msb64(subleaf_mask);
                } else if (!(subleaf_mask & (1 << subleaf))) {
                    /* invalid subleaf */
                    continue;
                }
                break;

            case 0xf:
                if (subleaf == 0) {
                    subleaf_mask |= r.edx;
                    max_subleaf = msb64(subleaf_mask);
                } else if (!(subleaf_mask & (1 << subleaf))) {
                    /* invalid subleaf */
                    continue;
                }
                break;

            case 0x10:
                if (subleaf == 0) {
                    subleaf_mask |= r.ebx;
                    max_subleaf = msb64(subleaf_mask);
                } else if (!(subleaf_mask & (1 << subleaf))) {
                    /* invalid subleaf */
                    continue;
                }
                break;
        }

        if ((r.eax || r.ebx || r.ecx || r.edx) == 0)
            return;

        if (!memcmp(&last_subleaf, &r, sizeof(last_subleaf)))
            return;

        print_subleaf(leaf, subleaf, r);
        last_subleaf = r;
    }
}

static void cpuid_level(uint32_t level) {
    cpuid_result r = do_cpuid(level, 0);

    int leaf;
    for (leaf = level; leaf <= r.eax; ++leaf) {
        cpuid_leaf(leaf);
    }
}

void
cpuid_subleaf(uint32_t leaf, uint32_t subleaf) {
    cpuid_result r = do_cpuid(leaf, subleaf);
    print_subleaf(leaf, subleaf, r);
}

void
cpuid_all() {
    cpuid_level(0);
    cpuid_level(0x80000000);
}
