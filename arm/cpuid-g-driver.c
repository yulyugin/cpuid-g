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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

#include "cpuid-g.h"
#include "asm-inline.h"

MODULE_LICENSE("BSD 2-Clause");
MODULE_AUTHOR("Evgeny Yulyugin <yulyugin@gmail.com>");
MODULE_DESCRIPTION("cpuid-g");

static int cpuid_g_open(struct inode *, struct file *);
static int cpuid_g_release(struct inode *, struct file *);
static ssize_t cpuid_g_read(struct file *, char *, size_t, loff_t *);

static atomic_t is_open = ATOMIC_INIT(0);

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = cpuid_g_read,
    .open = cpuid_g_open,
    .release = cpuid_g_release,
};

static struct miscdevice cpuid_g_dev = {
    MISC_DYNAMIC_MINOR,
    "cpuid-g",
    &fops
};

static int __init
cpuid_g_init(void)
{
    int ret = misc_register(&cpuid_g_dev);
    if (ret)
        printk(KERN_ERR "Unable to register cpuid-g device\n");

    return ret;
}

static void __exit
cpuid_g_exit(void)
{
    misc_deregister(&cpuid_g_dev);
}

module_init(cpuid_g_init);
module_exit(cpuid_g_exit);

static int
cpuid_g_open(struct inode *inode, struct file *file)
{
    if (atomic_add_unless(&is_open, 1, 1) == 0)
      return -EBUSY;
    return 0;
}

static int
cpuid_g_release(struct inode *inode, struct file *file)
{
    atomic_dec(&is_open);
    return 0;
}

#ifdef CONFIG_ARM64
arm64_cpuid_t arm64_cpuid;

static void
read_arm64_cpuid(void)
{
    arm64_cpuid.midr_el1 = MRS(MIDR_EL1);
    arm64_cpuid.mpidr_el1 = MRS(MPIDR_EL1);
    arm64_cpuid.revidr_el1 = MRS(REVIDR_EL1);

    arm64_cpuid.id_pfr0_el1 = MRS(ID_PFR0_EL1);
    arm64_cpuid.id_pfr1_el1 = MRS(ID_PFR1_EL1);
    arm64_cpuid.id_dfr0_el1 = MRS(ID_DFR0_EL1);
    arm64_cpuid.id_afr0_el1 = MRS(ID_AFR0_EL1);
    arm64_cpuid.id_mmfr0_el1 = MRS(ID_MMFR0_EL1);
    arm64_cpuid.id_mmfr1_el1 = MRS(ID_MMFR1_EL1);
    arm64_cpuid.id_mmfr2_el1 = MRS(ID_MMFR2_EL1);
    arm64_cpuid.id_mmfr3_el1 = MRS(ID_MMFR3_EL1);

    arm64_cpuid.id_isar0_el1 = MRS(ID_ISAR0_EL1);
    arm64_cpuid.id_isar1_el1 = MRS(ID_ISAR1_EL1);
    arm64_cpuid.id_isar2_el1 = MRS(ID_ISAR2_EL1);
    arm64_cpuid.id_isar3_el1 = MRS(ID_ISAR3_EL1);
    arm64_cpuid.id_isar4_el1 = MRS(ID_ISAR4_EL1);
    arm64_cpuid.id_isar5_el1 = MRS(ID_ISAR5_EL1);

    arm64_cpuid.id_aa64pfr0_el1 = MRS(ID_AA64PFR0_EL1);
    arm64_cpuid.id_aa64pfr1_el1 = MRS(ID_AA64PFR1_EL1);
    arm64_cpuid.id_aa64dfr0_el1 = MRS(ID_AA64DFR0_EL1);
    arm64_cpuid.id_aa64dfr1_el1 = MRS(ID_AA64DFR1_EL1);
    arm64_cpuid.id_aa64afr0_el1 = MRS(ID_AA64AFR0_EL1);
    arm64_cpuid.id_aa64afr1_el1 = MRS(ID_AA64AFR1_EL1);
    arm64_cpuid.id_aa64isar0_el1 = MRS(ID_AA64ISAR0_EL1);
    arm64_cpuid.id_aa64isar1_el1 = MRS(ID_AA64ISAR1_EL1);
    arm64_cpuid.id_aa64mmfr0_el1 = MRS(ID_AA64MMFR0_EL1);
    arm64_cpuid.id_aa64mmfr1_el1 = MRS(ID_AA64MMFR1_EL1);

    arm64_cpuid.ccsidr_el1 = MRS(CCSIDR_EL1);
    arm64_cpuid.clidr_el1 = MRS(CLIDR_EL1);
    arm64_cpuid.aidr_el1 = MRS(AIDR_EL1);
    arm64_cpuid.csselr_el1 = MRS(CSSELR_EL1);
    arm64_cpuid.ctr_el0 = MRS(CTR_EL0);
    arm64_cpuid.dczid_el0 = MRS(DCZID_EL0);
}

#else

arm32_cpuid_t arm32_cpuid;

static void
read_arm32_cpuid(void)
{
    uint32_t part_number;

    arm32_cpuid.midr = CR0_REG(0);
    arm32_cpuid.ctr = CR0_REG(1);
    arm32_cpuid.tcmtr = CR0_REG(2);
    arm32_cpuid.tlbtr = CR0_REG(3);

    part_number = arm32_cpuid.midr & ARM_CPU_PART_MASK;
    if (part_number == ARM_CPU_PART_ARM_CORTEX_A53) {
        arm32_cpuid.mpidr = CR0_REG(5);
        arm32_cpuid.revidr = CR0_REG(6);
    }

    arm32_cpuid.id_pfr0 = CR1_REG(0);
    arm32_cpuid.id_pfr1 = CR1_REG(1);
    arm32_cpuid.id_dfr0 = CR1_REG(2);
    arm32_cpuid.id_afr0 = CR1_REG(3);
    arm32_cpuid.id_mmfr0 = CR1_REG(4);
    arm32_cpuid.id_mmfr1 = CR1_REG(5);
    arm32_cpuid.id_mmfr2 = CR1_REG(6);
    arm32_cpuid.id_mmfr3 = CR1_REG(7);

    arm32_cpuid.id_isar0 = CR2_REG(0);
    arm32_cpuid.id_isar1 = CR2_REG(1);
    arm32_cpuid.id_isar2 = CR2_REG(2);
    arm32_cpuid.id_isar3 = CR2_REG(3);
    arm32_cpuid.id_isar4 = CR2_REG(4);
    arm32_cpuid.id_isar5 = CR2_REG(5);

    if (part_number == ARM_CPU_PART_ARM_CORTEX_A53) {
        arm32_cpuid.ccsidr = MRC_1_0(0);
        arm32_cpuid.clidr = MRC_1_0(1);
        arm32_cpuid.aidr = MRC_1_0(7);

        arm32_cpuid.csselr = MRC_2_0(0);
    }
}
#endif  /* CONFIG_ARM64 */

static ssize_t
cpuid_g_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int not_copied = 0;

#ifdef CONFIG_ARM64
    void *cpuid = (void *)&arm64_cpuid;
    unsigned long size = sizeof arm64_cpuid;
    read_arm64_cpuid();
#else
    void *cpuid = (void *)&arm32_cpuid;
    unsigned long size = sizeof arm32_cpuid;
    read_arm32_cpuid();
#endif

    if (length < 0)
        return 0;

    if (length > size)
        length = size;

    not_copied = copy_to_user(buffer, cpuid, length);

    return length - not_copied;
}
