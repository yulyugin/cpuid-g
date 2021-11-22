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

#ifndef ARM_ASM_INLINE_H
#define ARM_ASM_INLINE_H

#define MRC_OP1_CRM_OP2(op1, crm, op2)                \
({                                                    \
    uint32_t r;                                       \
    __asm__ __volatile__ (                            \
        "mrc p15, " #op1 ", %0, c0, c" #crm ", " #op2 \
        : "=r" (r)                                    \
        );                                            \
    r;                                                \
})

#define CR0_REG(op2) MRC_OP1_CRM_OP2(0, 0, op2)
#define CR1_REG(op2) MRC_OP1_CRM_OP2(0, 1, op2)
#define CR2_REG(op2) MRC_OP1_CRM_OP2(0, 2, op2)

#define MRC_1_0(op2) MRC_OP1_CRM_OP2(1, 0, op2)
#define MRC_2_0(op2) MRC_OP1_CRM_OP2(2, 0, op2)

#define MRS(id) ({         \
    uint64_t r;            \
    __asm__ __volatile__ ( \
        "mrs %0, " #id     \
        : "=r"(r));        \
    r;                     \
})

#endif  /* ARM_ASM_INLINE_H */
