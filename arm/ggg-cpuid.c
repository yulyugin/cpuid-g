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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int cpuids_num = 18;

const char *registers[] = {"Main ID Register",
                           "Cache Type Register",
                           "TCM Status Register",
                           "TLB Type Register",
                           "Processor Feature Register 0",
                           "Processor Feature Register 1",
                           "Debug Feature Register 0",
                           "Auxiliary Feature Register 0",
                           "Memory Model Feature Register 0",
                           "Memory Model Feature Register 1",
                           "Memory Model Feature Register 2",
                           "Memory Model Feature Register 3",
                           "Instruction Set Attributes Register 0",
                           "Instruction Set Attributes Register 1",
                           "Instruction Set Attributes Register 2",
                           "Instruction Set Attributes Register 3",
                           "Instruction Set Attributes Register 4",
                           "Instruction Set Attributes Register 5"
                           };

// Vendor definition
#define ARM     'A'  // 0x41
#define DEC     'D'  // 0x44
#define TI      'T'  // 0x54
#define INTEL   'i'  // 0x69

static uint32_t *get_cpuid() {
    int fd = open("/dev/ggg-cpuid", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return NULL;
    }
    uint32_t *id = (uint32_t *)calloc(cpuids_num, sizeof(uint32_t));
    if (read(fd, id, cpuids_num * 4) < 0) {
        perror("read");
        return NULL;
    }

    if (close(fd) < 0) {
        perror("close");
    }
    return id;
}

int main(int argc, char **argv) {
    uint32_t *c = get_cpuid();
    uint32_t implementer = c[0] >> 24;
    int i = 0;

    if (implementer == ARM)
        printf("Vendor: ARM\n");
    else if (implementer == DEC)
        printf("Vendor: DEC\n");
    else if (implementer == TI)
        printf("Vendor: Texas Instruments\n");
    else if (implementer == INTEL)
        printf("Vendor: Intel\n");

    for (i = 0; i < cpuids_num; ++i)
        printf("%-40s %#10x\n", registers[i], c[i]);

    free(c);
    return 0;
}
