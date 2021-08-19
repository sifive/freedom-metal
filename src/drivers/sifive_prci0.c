/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#if defined(METAL_SIFIVE_FU540_C000_PRCI0) ||                                  \
    defined(METAL_SIFIVE_FU740_C000_PRCI0)

#include <metal/drivers/sifive_prci0.h>
#include <metal/init.h>
#include <metal/machine.h>

/* Default setting of PLL */
#define PLL_R_default 0x1
#define PLL_F_default 0x1F
#define PLL_Q_default 0x3
#define PLL_RANGE_default 0x0
#define PLL_BYPASS_default 0x1
#define PLL_FSE_default 0x1

#define PLLOUT_DIV_default 0x0
#define PLLOUT_DIV_BY_1_default 0x0
#define PLLOUT_CLK_EN_default 0x0

/* Bit field of PLLCFG */
#define PLL_R(x) (((x)&0x3F) << 0)
#define PLL_F(x) (((x)&0x1FF) << 6)
#define PLL_Q(x) (((x)&0x7) << 15)
#define PLL_RANGE(x) (((x)&0x7) << 18)
#define PLL_BYPASS(x) (((x)&0x1) << 24)
#define PLL_FSE(x) (((x)&0x1) << 25)
#define PLL_LOCK(x) (((x)&0x1) << 31)

/* Bit field of PLLOUT */
#define PLLOUT_DIV(x) (((x)&0x7F) << 0)
#define PLLOUT_DIV_BY_1(x) (((x)&0x1) << 8)
#define PLLOUT_CLK_EN(x) (((x)&0x1) << 31)

/* Bit field of CORECLKSELREG (0x24) */
#define PLL_CORECLKSEL_HFXIN 0x1
#define PLL_CORECLKSEL_COREPLL 0x0

/* Bit field of CLKMUXSTATUS (0x2c) */
#define CLKMUX_STATUS_CORECLKPLLSEL (0x1 << 0)
#define CLKMUX_STATUS_TLCLKSEL (0x1 << 1)
#define CLKMUX_STATUS_RTCXSEL (0x1 << 2)
#define CLKMUX_STATUS_DDRCTRLCLKSEL (0x1 << 3)
#define CLKMUX_STATUS_DDRPHYCLKSEL (0x1 << 4)
#define CLKMUX_STATUS_GEMGXLCLKSEL (0x1 << 6)
#define CLKMUX_STATUS_MAINMEMCLKSEL (0x1 << 7)

/* Bit field of DEVICESRESETN (0x28) */
#define DEVICESRESET_DDR_CTRL_RST_N(x) (((x)&0x1) << 0)
#define DEVICESRESET_DDR_AXI_RST_N(x) (((x)&0x1) << 1)
#define DEVICESRESET_DDR_AHB_RST_N(x) (((x)&0x1) << 2)
#define DEVICESRESET_DDR_PHY_RST_N(x) (((x)&0x1) << 3)
#define DEVICESRESET_PCIE_POWER_UP_RST_N(x) (((x)&0x1) << 4)
#define DEVICESRESET_GEMGXL_RST_N(x) (((x)&0x1) << 5)
#define DEVICESRESET_CLTX_RST_N(x) (((x)&0x1) << 6)

/* Bit field of PRCI_PLLS (0xE0) */
#define PRCIPLL_CLTXPLL (0x1 << 0)
#define PRCIPLL_GEMGXLPLL (0x1 << 1)
#define PRCIPLL_DDRPLL (0x1 << 2)
#define PRCIPLL_HFPCLKPLL (0x1 << 3)
#define PRCIPLL_DVFSCOREPLL (0x1 << 4)
#define PRCIPLL_COREPLL (0x1 << 5)

#define PRCI_REG(x) __METAL_ACCESS_ONCE((__metal_io_u32 *)(control_base + x))

unsigned long __metal_driver_sifive_prci0_get_reg(struct metal_prci *prci,
                                                  unsigned long offset) {
    unsigned long control_base = __metal_driver_sifive_prci0_control_base(prci);
    if (!control_base)
        return 0;

    return PRCI_REG(offset);
}

unsigned long __metal_driver_sifive_prci0_set_reg(struct metal_prci *prci,
                                                  unsigned long offset,
                                                  unsigned long value) {
    unsigned long control_base = __metal_driver_sifive_prci0_control_base(prci);
    if (!control_base)
        return 0;

    return PRCI_REG(offset) = value;
}

#if defined(METAL_SIFIVE_FU540_C000_PRCI0)
void __metal_driver_sifive_prci0_init(void) {
    struct metal_prci *prci = metal_prci_get_device();
    unsigned long control_base = __metal_driver_sifive_prci0_control_base(prci);

    if (!control_base)
        return;

    /* CORE PLL */
    uint32_t coreHz;
    if (PRCI_REG(METAL_SIFIVE_PRCI0_CLKMUXSTATUS) & CLKMUX_STATUS_TLCLKSEL) {
        /* 500 MHz */
        coreHz = (PLL_R(0)) | (PLL_F(59)) | // 4000MHz VCO
                 (PLL_Q(3)) |               // /8 Output divider
                 (PLL_RANGE(0x4)) | (PLL_BYPASS(0)) | (PLL_FSE(1));
    } else {
        /* 1 GHz */
        coreHz = (PLL_R(0)) | (PLL_F(59)) | // 4000MHz VCO
                 (PLL_Q(2)) |               // /4 Output divider
                 (PLL_RANGE(0x4)) | (PLL_BYPASS(0)) | (PLL_FSE(1));
    }

    PRCI_REG(METAL_SIFIVE_PRCI0_COREPLLCFG) = coreHz;

    /* wait for lock */
    while ((PRCI_REG(METAL_SIFIVE_PRCI0_COREPLLCFG) & (PLL_LOCK(1))) == 0)
        ;

    uint32_t core_out = (PLLOUT_DIV(PLLOUT_DIV_default)) |
                        (PLLOUT_DIV_BY_1(PLLOUT_DIV_BY_1_default)) |
                        (PLLOUT_CLK_EN(1));

    PRCI_REG(METAL_SIFIVE_PRCI0_COREPLLOUTDIV) = core_out;

    /* set CORECLKSELREG to select COREPLL */
    PRCI_REG(METAL_SIFIVE_PRCI0_CORECLKSELREG) = PLL_CORECLKSEL_COREPLL;

    /* GEMGXL PLL */
    uint32_t gemgxl125mhz = (PLL_R(0)) | (PLL_F(59)) | // 4000Mhz VCO
                            (PLL_Q(5)) |               // /32
                            (PLL_RANGE(0x4)) | (PLL_BYPASS(0)) | (PLL_FSE(1));

    PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLCFG) = gemgxl125mhz;

    /* wait for lock */
    while ((PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLCFG) & PLL_LOCK(1)) == 0)
        ;

    uint32_t gemgxlctl_out = (PLLOUT_DIV(PLLOUT_DIV_default)) |
                             (PLLOUT_DIV_BY_1(PLLOUT_DIV_BY_1_default)) |
                             (PLLOUT_CLK_EN(1));

    PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLOUTDIV) = gemgxlctl_out;

    /* release gemgxl reset */
    PRCI_REG(METAL_SIFIVE_PRCI0_DEVICESRESETN) |= DEVICESRESET_GEMGXL_RST_N(1);

    /* procmon => core clock */
    PRCI_REG(METAL_SIFIVE_PRCI0_PROCMONCFG) = 0x1 << 24;
}
#elif defined(METAL_SIFIVE_FU740_C000_PRCI0)
void __metal_driver_sifive_prci0_init(void) {
    struct metal_prci *prci = metal_prci_get_device();
    unsigned long control_base = __metal_driver_sifive_prci0_control_base(prci);

    if (!control_base)
        return;

    /* CORE PLL */
    uint32_t coreHz;
    if (PRCI_REG(METAL_SIFIVE_PRCI0_CLKMUXSTATUS) & CLKMUX_STATUS_TLCLKSEL) {
        /* 500 MHz */
        coreHz = (PLL_R(0)) | (PLL_F(76)) | // 4000MHz VCO
                 (PLL_Q(3)) |               // /8 Output divider
                 (PLL_RANGE(0x3)) |         // 26MHz is between 18-38MHz
                 (PLL_BYPASS(0)) | (PLL_FSE(1));
    } else {
        /* 1 GHz */
        coreHz = (PLL_R(0)) | (PLL_F(76)) | // 4000MHz VCO
                 (PLL_Q(2)) |               // /4 Output divider
                 (PLL_RANGE(0x3)) |         // 26MHz is between 18-38MHz
                 (PLL_BYPASS(0)) | (PLL_FSE(1));
    }

    PRCI_REG(METAL_SIFIVE_PRCI0_COREPLLCFG) = coreHz;

    /* wait for lock */
    while ((PRCI_REG(METAL_SIFIVE_PRCI0_COREPLLCFG) & (PLL_LOCK(1))) == 0)
        ;

    uint32_t core_out = (PLLOUT_DIV(PLLOUT_DIV_default)) |
                        (PLLOUT_DIV_BY_1(PLLOUT_DIV_BY_1_default)) |
                        (PLLOUT_CLK_EN(1));

    PRCI_REG(METAL_SIFIVE_PRCI0_COREPLLOUTDIV) = core_out;

    /* set CORECLKSELREG to select COREPLL */
    PRCI_REG(METAL_SIFIVE_PRCI0_CORECLKSELREG) = PLL_CORECLKSEL_COREPLL;

    if (PRCI_REG(METAL_SIFIVE_PRCI0_PRCIPLLS) & PRCIPLL_HFPCLKPLL) {
        /* HFPCLK PLL */
        uint32_t hfpclk250mhz = (PLL_R(0)) | (PLL_F(79)) | // 4000MHz VCO
                                (PLL_Q(4)) |               // /16
                                (PLL_RANGE(0x3)) | (PLL_BYPASS(1)) |
                                (PLL_FSE(1));

        PRCI_REG(METAL_SIFIVE_PRCI0_HFPCLKPLLCFG) |= PLL_BYPASS(1);
        PRCI_REG(METAL_SIFIVE_PRCI0_HFPCLKPLLCFG) = hfpclk250mhz;
        PRCI_REG(METAL_SIFIVE_PRCI0_HFPCLKPLLCFG) &= ~(PLL_BYPASS(1));

        /* wait for lock */
        while ((PRCI_REG(METAL_SIFIVE_PRCI0_HFPCLKPLLCFG) & PLL_LOCK(1)) == 0)
            ;

        uint32_t hfpclkpll_out = (PLLOUT_DIV(PLLOUT_DIV_default)) |
                                 (PLLOUT_DIV_BY_1(PLLOUT_DIV_BY_1_default)) |
                                 (PLLOUT_CLK_EN(1));

        PRCI_REG(METAL_SIFIVE_PRCI0_HFPCLKPLLOUTDIV) = hfpclkpll_out;

        /* switch pclk to run from HFPCLKPLL instead of the 26MHz hfclk */
        PRCI_REG(METAL_SIFIVE_PRCI0_HFPCLKPLLSEL) = 0;
    } else if (PRCI_REG(METAL_SIFIVE_PRCI0_PRCIPLLS) & PRCIPLL_CLTXPLL) {
        /* CLTX PLL */
        uint32_t cltx250mhz = (PLL_R(0)) | (PLL_F(79)) | // 4000MHz VCO
                              (PLL_Q(4)) |               // /16
                              (PLL_RANGE(0x3)) | (PLL_BYPASS(1)) | (PLL_FSE(1));

        PRCI_REG(METAL_SIFIVE_PRCI0_CLTXPLLCFG) |= PLL_BYPASS(1);
        PRCI_REG(METAL_SIFIVE_PRCI0_CLTXPLLCFG) = cltx250mhz;
        PRCI_REG(METAL_SIFIVE_PRCI0_CLTXPLLCFG) &= ~(PLL_BYPASS(1));

        /* wait for lock */
        while ((PRCI_REG(METAL_SIFIVE_PRCI0_CLTXPLLCFG) & PLL_LOCK(1)) == 0)
            ;

        uint32_t cltxctl_out = (PLLOUT_DIV(PLLOUT_DIV_default)) |
                               (PLLOUT_DIV_BY_1(PLLOUT_DIV_BY_1_default)) |
                               (PLLOUT_CLK_EN(1));

        PRCI_REG(METAL_SIFIVE_PRCI0_CLTXPLLOUTDIV) = cltxctl_out;
    }

    /* GEMGXL PLL */
    uint32_t gemgxl125mhz = (PLL_R(0)) | (PLL_F(76)) | // 4000Mhz VCO
                            (PLL_Q(5)) |               // /32
                            (PLL_RANGE(0x3)) | (PLL_BYPASS(1)) | (PLL_FSE(1));

    PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLCFG) |= PLL_BYPASS(1);
    PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLCFG) = gemgxl125mhz;
    PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLCFG) &= ~(PLL_BYPASS(1));

    /* wait for lock */
    while ((PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLCFG) & PLL_LOCK(1)) == 0)
        ;

    uint32_t gemgxlctl_out = (PLLOUT_DIV(PLLOUT_DIV_default)) |
                             (PLLOUT_DIV_BY_1(PLLOUT_DIV_BY_1_default)) |
                             (PLLOUT_CLK_EN(1));

    PRCI_REG(METAL_SIFIVE_PRCI0_GEMGXLPLLOUTDIV) = gemgxlctl_out;

    /* release gemgxl reset */
    PRCI_REG(METAL_SIFIVE_PRCI0_DEVICESRESETN) |= DEVICESRESET_GEMGXL_RST_N(1);

    /* procmon => core clock */
    PRCI_REG(METAL_SIFIVE_PRCI0_PROCMONCFG) = 0x1 << 24;

    /* release Chiplink reset */
    PRCI_REG(METAL_SIFIVE_PRCI0_DEVICESRESETN) |= DEVICESRESET_CLTX_RST_N(1);
}
#endif

METAL_CONSTRUCTOR(metal_sifive_prci_init) {
    __metal_driver_sifive_prci0_init();
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_prci0) = {
    .prci.get_reg = __metal_driver_sifive_prci0_get_reg,
    .prci.set_reg = __metal_driver_sifive_prci0_set_reg,
};

#endif /* METAL_SIFIVE_FU540_C000_PRCI0 || METAL_SIFIVE_FU740_C000_PRCI0 */

/* From 545bbbf ("Fix empty translation unit warning") ? */
typedef int no_empty_translation_units;
