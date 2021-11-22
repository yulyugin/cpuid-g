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

#include "cpuid-g.h"

// Vendor definition
#define ARM     'A'  // 0x41
#define DEC     'D'  // 0x44
#define TI      'T'  // 0x54
#define INTEL   'i'  // 0x69

#define PRINT_CPUID_REG(name_string, name) \
    printf("%-40s %#10x\n", name_string, c->name);

static int
get_cpuid(arm32_cpuid_t *id)
{
    int fd = open("/dev/cpuid-g", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (read(fd, id, sizeof *id) < 0) {
        perror("read");
        return -1;
    }

    if (close(fd) < 0) {
        perror("close");
    }
    return 0;
}

static int
print_arm32_cpuid(void)
{
    arm32_cpuid_t *c = (arm32_cpuid_t *)calloc(1, sizeof *c);
    if (!c) {
        perror("calloc");
        return 1;
    }

    if (get_cpuid(c) < 0)
        return 1;

    uint32_t implementer = c->midr >> 24;
    if (implementer == ARM)
        printf("Vendor: ARM\n");
    else if (implementer == DEC)
        printf("Vendor: DEC\n");
    else if (implementer == TI)
        printf("Vendor: Texas Instruments\n");
    else if (implementer == INTEL)
        printf("Vendor: Intel\n");

    uint32_t part_number = c->midr & ARM_CPU_PART_MASK;

    PRINT_CPUID_REG("Main ID Register", midr);
    PRINT_CPUID_REG("Cache Type Register", ctr);
    PRINT_CPUID_REG("TCM Type Register", tcmtr);
    PRINT_CPUID_REG("TLB Type Register", tlbtr);
    if (part_number == ARM_CPU_PART_ARM_CORTEX_A53) {
        PRINT_CPUID_REG("Multiprocessor Affinity Register", mpidr);
        PRINT_CPUID_REG("Revision ID Register", revidr);
    }

    PRINT_CPUID_REG("Processor Feature Register 0", id_pfr0);
    PRINT_CPUID_REG("Processor Feature Register 1", id_pfr1);
    PRINT_CPUID_REG("Debug Feature Register 0", id_dfr0);
    PRINT_CPUID_REG("Auxiliary Feature Register 0", id_afr0);
    PRINT_CPUID_REG("Memory Model Feature Register 0", id_mmfr0);
    PRINT_CPUID_REG("Memory Model Feature Register 1", id_mmfr1);
    PRINT_CPUID_REG("Memory Model Feature Register 2", id_mmfr2);
    PRINT_CPUID_REG("Memory Model Feature Register 3", id_mmfr3);

    PRINT_CPUID_REG("Instruction Set Attributes Register 0", id_isar0);
    PRINT_CPUID_REG("Instruction Set Attributes Register 1", id_isar1);
    PRINT_CPUID_REG("Instruction Set Attributes Register 2", id_isar2);
    PRINT_CPUID_REG("Instruction Set Attributes Register 3", id_isar3);
    PRINT_CPUID_REG("Instruction Set Attributes Register 4", id_isar4);
    PRINT_CPUID_REG("Instruction Set Attributes Register 5", id_isar5);

    if (part_number == ARM_CPU_PART_ARM_CORTEX_A53) {
        PRINT_CPUID_REG("Cache Size ID Register", ccsidr);
        PRINT_CPUID_REG("Cache Level ID Register", clidr);
        PRINT_CPUID_REG("Auxiliary ID Register", aidr);

        PRINT_CPUID_REG("Cache Size Selection Reigster", csselr);
    }

    free(c);
}

static int
print_arm64_cpuid()
{
    return 0;
}

int
main(int argc, char **argv)
{
    if (sizeof(void *) == 8)
        return print_arm64_cpuid();
    else
        return print_arm32_cpuid();
}
