/* Print CPUID the right way
 *
 * Copyright (c) 2014, Evgenii Iuliugin.
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

#define PRINT_CPUID_REG32(name_string, name) \
    printf("%-40s %#18x\n", name_string, c->name);

#define PRINT_CPUID_REG64(name_string, name) \
    printf("%-40s %#18llx\n", name_string, c->name);

static int
get_cpuid(void *id, size_t size)
{
    int fd = open("/dev/cpuid-g", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (read(fd, id, size) < 0) {
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

    if (get_cpuid(c, sizeof *c) < 0)
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

    PRINT_CPUID_REG32("Main ID", midr);
    PRINT_CPUID_REG32("Cache Type", ctr);
    PRINT_CPUID_REG32("TCM Type", tcmtr);
    PRINT_CPUID_REG32("TLB Type", tlbtr);
    if (part_number == ARM_CPU_PART_ARM_CORTEX_A53) {
        PRINT_CPUID_REG32("Multiprocessor Affinity", mpidr);
        PRINT_CPUID_REG32("Revision ID", revidr);
    }

    PRINT_CPUID_REG32("Processor Feature 0", id_pfr0);
    PRINT_CPUID_REG32("Processor Feature 1", id_pfr1);
    PRINT_CPUID_REG32("Debug Feature 0", id_dfr0);
    PRINT_CPUID_REG32("Auxiliary Feature 0", id_afr0);
    PRINT_CPUID_REG32("Memory Model Feature 0", id_mmfr0);
    PRINT_CPUID_REG32("Memory Model Feature 1", id_mmfr1);
    PRINT_CPUID_REG32("Memory Model Feature 2", id_mmfr2);
    PRINT_CPUID_REG32("Memory Model Feature 3", id_mmfr3);

    PRINT_CPUID_REG32("Instruction Set Attributes 0", id_isar0);
    PRINT_CPUID_REG32("Instruction Set Attributes 1", id_isar1);
    PRINT_CPUID_REG32("Instruction Set Attributes 2", id_isar2);
    PRINT_CPUID_REG32("Instruction Set Attributes 3", id_isar3);
    PRINT_CPUID_REG32("Instruction Set Attributes 4", id_isar4);
    PRINT_CPUID_REG32("Instruction Set Attributes 5", id_isar5);

    if (part_number == ARM_CPU_PART_ARM_CORTEX_A53) {
        PRINT_CPUID_REG32("Cache Size ID", ccsidr);
        PRINT_CPUID_REG32("Cache Level ID", clidr);
        PRINT_CPUID_REG32("Auxiliary ID", aidr);

        PRINT_CPUID_REG32("Cache Size Selection", csselr);
    }

    free(c);
}

static int
print_arm64_cpuid()
{
    arm64_cpuid_t *c = (arm64_cpuid_t *)calloc(1, sizeof *c);
    if (!c) {
        perror("calloc");
        return 1;
    }

    if (get_cpuid(c, sizeof *c) < 0)
        return 1;

    PRINT_CPUID_REG32("Main ID", midr_el1);
    PRINT_CPUID_REG64("Multiprocessor Affinity", mpidr_el1);
    PRINT_CPUID_REG32("Revision ID", revidr_el1);

    PRINT_CPUID_REG32("Aarch32 Processor Feature 0", id_pfr0_el1);
    PRINT_CPUID_REG32("Aarch32 Processor Feature 1", id_pfr1_el1);
    PRINT_CPUID_REG32("Aarch32 Debug Feature 0", id_dfr0_el1);
    PRINT_CPUID_REG32("Aarch32 Auxiliary Feature 0", id_afr0_el1);
    PRINT_CPUID_REG32("Aarch32 Memory Model Feature 0", id_mmfr0_el1);
    PRINT_CPUID_REG32("Aarch32 Memory Model Feature 1", id_mmfr1_el1);
    PRINT_CPUID_REG32("Aarch32 Memory Model Feature 2", id_mmfr2_el1);
    PRINT_CPUID_REG32("Aarch32 Memory Model Feature 3", id_mmfr3_el1);

    PRINT_CPUID_REG32("Aarch32 Instruction Set Attribute 0", id_isar0_el1);
    PRINT_CPUID_REG32("Aarch32 Instruction Set Attribute 1", id_isar1_el1);
    PRINT_CPUID_REG32("Aarch32 Instruction Set Attribute 2", id_isar2_el1);
    PRINT_CPUID_REG32("Aarch32 Instruction Set Attribute 3", id_isar3_el1);
    PRINT_CPUID_REG32("Aarch32 Instruction Set Attribute 4", id_isar4_el1);
    PRINT_CPUID_REG32("Aarch32 Instruction Set Attribute 5", id_isar5_el1);

    PRINT_CPUID_REG64("Aarch64 Processor Feature 0", id_aa64pfr0_el1);
    PRINT_CPUID_REG64("Aarch64 Processor Feature 1", id_aa64pfr1_el1);
    PRINT_CPUID_REG64("Aarch64 Debug Feature 0", id_aa64dfr0_el1);
    PRINT_CPUID_REG64("Aarch64 Debug Feature 1", id_aa64dfr1_el1);
    PRINT_CPUID_REG64("Aarch64 Auxuliary Feature 0", id_aa64afr0_el1);
    PRINT_CPUID_REG64("Aarch64 Auxuliary Feature 1", id_aa64afr1_el1);
    PRINT_CPUID_REG64("Aarch64 Instruction Set Attribute 0", id_aa64isar0_el1);
    PRINT_CPUID_REG64("Aarch64 Instruction Set Attribute 1", id_aa64isar1_el1);
    PRINT_CPUID_REG64("Aarch64 Memory Model Feature 0", id_aa64mmfr0_el1);
    PRINT_CPUID_REG64("Aarch64 Memory Model Feature 1", id_aa64mmfr1_el1);

    PRINT_CPUID_REG32("Cache Size ID Registetr", ccsidr_el1);
    PRINT_CPUID_REG64("Cache Level ID", clidr_el1);
    PRINT_CPUID_REG32("Auxiliary ID", aidr_el1);
    PRINT_CPUID_REG32("Cache Size Selection", csselr_el1);
    PRINT_CPUID_REG32("Cache Type", ctr_el0);
    PRINT_CPUID_REG32("Data Cache Zero ID", dczid_el0);

    free(c);
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
