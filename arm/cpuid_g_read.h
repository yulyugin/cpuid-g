
#ifdef CONFIG_ARM64
arm64_cpuid_t arm64_cpuid;

static void
read_arm64_cpuid(void)
{
    arm64_cpuid.midr = MRS(MIDR_EL1);
    arm64_cpuid.mpidr = MRS(MPIDR_EL1);
    arm64_cpuid.revidr = MRS(REVIDR_EL1);

    arm64_cpuid.id_pfr0 = MRS(ID_PFR0_EL1);
    arm64_cpuid.id_pfr1 = MRS(ID_PFR1_EL1);
    arm64_cpuid.id_dfr0 = MRS(ID_DFR0_EL1);
    arm64_cpuid.id_afr0 = MRS(ID_AFR0_EL1);
    arm64_cpuid.id_mmfr0 = MRS(ID_MMFR0_EL1);
    arm64_cpuid.id_mmfr1 = MRS(ID_MMFR1_EL1);
    arm64_cpuid.id_mmfr2 = MRS(ID_MMFR2_EL1);
    arm64_cpuid.id_mmfr3 = MRS(ID_MMFR3_EL1);

    arm64_cpuid.id_isar0 = MRS(ID_ISAR0_EL1);
    arm64_cpuid.id_isar1 = MRS(ID_ISAR1_EL1);
    arm64_cpuid.id_isar2 = MRS(ID_ISAR2_EL1);
    arm64_cpuid.id_isar3 = MRS(ID_ISAR3_EL1);
    arm64_cpuid.id_isar4 = MRS(ID_ISAR4_EL1);
    arm64_cpuid.id_isar5 = MRS(ID_ISAR5_EL1);

    arm64_cpuid.id_aa64pfr0 = MRS(ID_AA64PFR0_EL1);
    arm64_cpuid.id_aa64pfr1 = MRS(ID_AA64PFR1_EL1);
    arm64_cpuid.id_aa64dfr0 = MRS(ID_AA64DFR0_EL1);
    arm64_cpuid.id_aa64dfr1 = MRS(ID_AA64DFR1_EL1);
    arm64_cpuid.id_aa64afr0 = MRS(ID_AA64AFR0_EL1);
    arm64_cpuid.id_aa64afr1 = MRS(ID_AA64AFR1_EL1);
    arm64_cpuid.id_aa64isar0 = MRS(ID_AA64ISAR0_EL1);
    arm64_cpuid.id_aa64isar1 = MRS(ID_AA64ISAR1_EL1);
    arm64_cpuid.id_aa64mmfr0 = MRS(ID_AA64MMFR0_EL1);
    arm64_cpuid.id_aa64mmfr1 = MRS(ID_AA64MMFR1_EL1);

    arm64_cpuid.ccsidr = MRS(CCSIDR_EL1);
    arm64_cpuid.clidr = MRS(CLIDR_EL1);
    arm64_cpuid.aidr = MRS(AIDR_EL1);
    arm64_cpuid.csselr = MRS(CSSELR_EL1);
    arm64_cpuid.ctr = MRS(CTR_EL0);
    arm64_cpuid.dczid = MRS(DCZID_EL0);
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

