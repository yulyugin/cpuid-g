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

#ifndef ARM_CPUID_G_H
#define ARM_CPUID_G_H

#define ARM_CPU_PART_MASK           0xff00fff0

#define ARM_CPU_PART_ARM1176        0x4100b760
#define ARM_CPU_PART_ARM_CORTEX_A53 0x4100d030

typedef struct arm32_cpuid {
    uint32_t midr;
    uint32_t ctr;
    uint32_t tcmtr;
    uint32_t tlbtr;
    uint32_t mpidr;
    uint32_t revidr;

    uint32_t id_pfr0;
    uint32_t id_pfr1;
    uint32_t id_dfr0;
    uint32_t id_afr0;
    uint32_t id_mmfr0;
    uint32_t id_mmfr1;
    uint32_t id_mmfr2;
    uint32_t id_mmfr3;

    uint32_t id_isar0;
    uint32_t id_isar1;
    uint32_t id_isar2;
    uint32_t id_isar3;
    uint32_t id_isar4;
    uint32_t id_isar5;

    uint32_t ccsidr;
    uint32_t clidr;
    uint32_t aidr;

    uint32_t csselr;
} arm32_cpuid_t;

typedef struct arm64_cpuid {
    uint32_t midr_el1;
    uint64_t mpidr_el1;
    uint32_t revidr_el1;

    uint32_t id_pfr0_el1;
    uint32_t id_pfr1_el1;
    uint32_t id_dfr0_el1;
    uint32_t id_afr0_el1;
    uint32_t id_mmfr0_el1;
    uint32_t id_mmfr1_el1;
    uint32_t id_mmfr2_el1;
    uint32_t id_mmfr3_el1;

    uint32_t id_isar0_el1;
    uint32_t id_isar1_el1;
    uint32_t id_isar2_el1;
    uint32_t id_isar3_el1;
    uint32_t id_isar4_el1;
    uint32_t id_isar5_el1;

    uint64_t id_aa64pfr0_el1;
    uint64_t id_aa64pfr1_el1;
    uint64_t id_aa64dfr0_el1;
    uint64_t id_aa64dfr1_el1;
    uint64_t id_aa64afr0_el1;
    uint64_t id_aa64afr1_el1;
    uint64_t id_aa64isar0_el1;
    uint64_t id_aa64isar1_el1;
    uint64_t id_aa64mmfr0_el1;
    uint64_t id_aa64mmfr1_el1;

    uint32_t ccsidr_el1;
    uint64_t clidr_el1;
    uint32_t aidr_el1;
    uint32_t csselr_el1;
    uint32_t ctr_el0;
    uint32_t dczid_el0;
} arm64_cpuid_t;

#endif  /* ARM_CPUID_G_H */
