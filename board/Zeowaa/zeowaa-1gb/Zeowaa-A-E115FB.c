// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018 Microsemi Corporation.
 * Padmarao Begari, Microsemi Corporation <padmarao.begari@microsemi.com>
 * Copyright (C) 2018 Joey Hewitt <joey@joeyhewitt.com>
 * Copuright (C) 2019 Anatoly Trosinenko (adapted ../../SiFive/HiFive-U540/HiFive-U540.c)
 */

#include <asm/mach-types.h>
#include <common.h>
#if defined(CONFIG_MACB) && !defined(CONFIG_DM_ETH)
#include <netdev.h>
#endif
#include <linux/io.h>
#include <dm/uclass.h>
#include <dm/device.h>
#include <misc.h>
#include <inttypes.h>

DECLARE_GLOBAL_DATA_PTR;

struct hifive_gpio_regs
{
    volatile uint32_t  INPUT_VAL;   /* 0x0000 */
    volatile uint32_t  INPUT_EN;    /* 0x0004 */
    volatile uint32_t  OUTPUT_VAL;  /* 0x0008 */
    volatile uint32_t  OUTPUT_EN;   /* 0x000C */
    volatile uint32_t  PUE;         /* 0x0010 */
    volatile uint32_t  DS;          /* 0x0014 */
    volatile uint32_t  RISE_IE;     /* 0x0018 */
    volatile uint32_t  RISE_IP;     /* 0x001C */
    volatile uint32_t  FALL_IE;     /* 0x0020 */
    volatile uint32_t  FALL_IP;     /* 0x0024 */
    volatile uint32_t  HIGH_IE;     /* 0x0028 */
    volatile uint32_t  HIGH_IP;     /* 0x002C */
    volatile uint32_t  LOW_IE;      /* 0x0030 */
    volatile uint32_t  LOW_IP;      /* 0x0034 */
    volatile uint32_t  reserved0;   /* 0x0038 */
    volatile uint32_t  reserved1;   /* 0x003C */
    volatile uint32_t  OUT_XOR;     /* 0x0040 */
};


/*
 * Miscellaneous platform dependent initializations
 */

int board_init(void)
{
	gd->bd->bi_arch_number = MACH_TYPE_HIFIVE_U540;
	gd->bd->bi_boot_params = PHYS_SDRAM_0;

	return 0;
}

int dram_init(void)
{
	unsigned long sdram_base = PHYS_SDRAM_0;
	unsigned long expected_size = PHYS_SDRAM_0_SIZE;/* + PHYS_SDRAM_1_SIZE;*/
	unsigned long actual_size;

	actual_size = get_ram_size((void *)sdram_base, expected_size);
	gd->ram_size = actual_size;

	if (expected_size != actual_size) {
		printf("Warning: Only %lu of %lu MiB SDRAM is working\n",
			actual_size >> 20, expected_size >> 20);
	}
	gd->fdt_size = ALIGN(fdt_totalsize(gd->fdt_blob) + 0x1000, 32);
	memcpy((void *)HIFIVE_FDT_BASE, gd->fdt_blob, gd->fdt_size);

	return 0;
}

void reset_phy(void)
{
}
int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_0;
	gd->bd->bi_dram[0].size =  PHYS_SDRAM_0_SIZE;

	return 0;
}

/* This define is a value used for error/unknown serial. If we really care about distinguishing errors and 0 is valid, we'll need a different one. */
#define ERROR_READING_SERIAL_NUMBER        0

#ifdef CONFIG_MISC_INIT_R
static u32 setup_serialnum(void);
static void setup_macaddr(u32 serialnum);

int misc_init_r(void)
{
	if (!env_get("serial#")) {
		u32 serialnum = setup_serialnum();
		setup_macaddr(serialnum);
	}
	return 0;
}
#endif

#if defined(CONFIG_MACB) && !defined(CONFIG_DM_ETH)
int board_eth_init(bd_t *bd)
{
	int rc = 0;

	rc = macb_eth_initialize(0, (void *)HIFIVE_BASE_ETHERNET, 0x00);

	return rc;
}
#endif
ulong board_flash_get_legacy(ulong base, int banknum, flash_info_t *info)
{
	return 0;
}

static u32 setup_serialnum(void)
{
	u32 serial = ERROR_READING_SERIAL_NUMBER;
	char serial_str[6];

	return serial;
}

static void setup_macaddr(u32 serialnum) {
	// OR the serial into the MAC -- see SiFive FSBL
	unsigned char mac[6] = { 0x70, 0xb3, 0xd5, 0x92, 0xf0, 0x00 };
	mac[5] |= (serialnum >>  0) & 0xff;
	mac[4] |= (serialnum >>  8) & 0xff;
	mac[3] |= (serialnum >> 16) & 0xff;
	eth_env_set_enetaddr("ethaddr", mac);
}
