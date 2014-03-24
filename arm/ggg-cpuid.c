/* Print CPUID the right way
 *
 * Copyright (c) 2014, Evgeny Yulyugin.
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

union cpuid_t {
    struct {
        unsigned revision:4;
        unsigned part_no:12;
        unsigned architecture_variant:8;
        unsigned implementer:8;
    };

    uint32_t cpuid;
};

// Vendor definition
#define ARM     'A'  // 0x41
#define DEC     'D'  // 0x44
#define TI      'T'  // 0x54
#define INTEL   'i'  // 0x69

static uint32_t get_cpuid() {
    uint32_t id;
    __asm__ __volatile__ (
        "mrc p15 0, %0, c0, c0, 0"
        : "=r" (id)
        );

    return id;
}

int main(int argc, char **argv) {
    union cpuid_t c;
    c.cpuid = get_cpuid();

    printf("CPUID value: %#10x", c.cpuid);
    if (c.implementer == ARM)
        printf("Vendor: ARM\n");
    else if (c.implementer == DEC)
        printf("Vendor: DEC\n");
    else if (c.implementer == TI)
        printf("Vendor: Texas Instruments\n");
    else if (c.implementer == INTEL)
        printf("Vendor: Intel\n");

    return 0;
}