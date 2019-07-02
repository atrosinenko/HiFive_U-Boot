/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2018 Microsemi Corporation.
 * Padmarao Begari, Microsemi Corporation <padmarao.begari@microsemi.com>
 * Copyright (C) 2018 SiFive Inc
 * Troy Benjegerdes, <troy.benjegerdes@sifive.com>
 * Copyright (C) 2019 Anatoly Trosinenko (adapted HiFive-U540.h)
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef CONFIG_OF_CONTROL
#undef CONFIG_OF_SEPARATE
#define CONFIG_OF_EMBED
#endif

#include <version.h>

/*
 * CPU and Board Configuration Options
 */

#define CONFIG_BOOTP_SEND_HOSTNAME
//#define CONFIG_BOOTP_SERVERIP

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_CBSIZE	1024	/* Console I/O Buffer Size */

/*
 * Print Buffer Size
 */
#define CONFIG_SYS_PBSIZE	\
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/*
 * max number of command args
 */
#define CONFIG_SYS_MAXARGS	16

/*
 * Boot Argument Buffer Size
 */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

/*
 * Size of malloc() pool
 * 512kB is suggested, (CONFIG_ENV_SIZE + 128 * 1024) was not enough
 */
#define CONFIG_SYS_MALLOC_LEN   (512 << 10)

#define CONFIG_SYS_CLK_FREQ    		25000000

/*
 * Physical Memory Map
 */
/*#define CONFIG_NR_DRAM_BANKS	1*/
#define PHYS_SDRAM_0	0x80000000		/* SDRAM Bank #1 */
#define PHYS_SDRAM_0_SIZE	0x40000000	/* 1 GB */
#define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_0
/*
 * Serial console configuration
 */
#define HIFIVE_UART_BASE_ADDR	0x64000000
#define HIFIVE_PERIPH_CLK_FREQ	(CONFIG_SYS_CLK_FREQ / 1)
#define CONSOLE_ARG				"console=ttyS0,115200\0"

/* Init Stack Pointer */
#define CONFIG_SYS_INIT_SP_ADDR		(0x80000000 + 0x011D0000 - \
					GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_LOAD_ADDR		0xa0000000	/* partway up SDRAM */

#define HIFIVE_BASE_CLINT		0x02000000
#define HIFIVE_BASE_MTIME		0x0200bff8
#define HIFIVE_BASE_TIMECMP		0x02004000
//#define HIFIVE_BASE_PRCI		0x10000000
#define HIFIVE_BASE_SPI			0x64001000
#define HIFIVE_BASE_GPIO	 	0x64002000
//#define HIFIVE_BASE_ETHERNET	0x10090000
//#define HIFIVE_BASE_MAC_MGMT	0x100a0000

#define HIFIVE_ETHERNET_CLK_FREQ	(CONFIG_SYS_CLK_FREQ / 1)

#define CONFIG_HIFIVE_SPI
#define CONFIG_MMC_SPI
#define CONFIG_SPI
#define CONFIG_SYS_SPI_BASE			HIFIVE_BASE_SPI
#define CONFIG_SYS_SPI_CLK			(CONFIG_SYS_CLK_FREQ / 1)
#define CONFIG_ENV_SPI_MAX_HZ		10000000

#define CONFIG_BOOTFILE			"hifiveu.fit"

/*
 * Ethernet
 */
#ifdef CONFIG_CMD_NET
#define CONFIG_MACB
#define CONFIG_GMII
#define CONFIG_NET_RETRY_COUNT		20
#define CONFIG_MACB_SEARCH_PHY
#define CONFIG_ARP_TIMEOUT		200UL
#define CONFIG_IP_DEFRAG
#define CONFIG_CMD_MII
#define CONFIG_RESET_PHY_R

#endif

#define HIFIVE_FDT_BASE 0xb0000000

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Initial Memory map for Linux*/
#define CONFIG_SYS_BOOTMAPSZ    (128 << 20)
/* Increase max gunzip size */
#define CONFIG_SYS_BOOTM_LEN    (128 << 20)

/* Enable distro boot */
#define BOOT_TARGET_DEVICES(func) \
    func(MMC, mmc, 0) 
#include <config_distro_bootcmd.h>

#define	CONFIG_EXTRA_ENV_SETTINGS	\
		"uboot_version=" __stringify(PLAIN_VERSION) "\0" \
		"mmcsetup=mmc_spi 1 10000000 0; mmc part\0" \
		"fdt_high=0xffffffffffffffff\0" \
		"fdtsetup=fdt addr ${fdtcontroladdr}; fdt chosen;" \
			"fdt set /firmware sifive,uboot " __stringify(PLAIN_VERSION) ";" \
			"fdt set /chosen bootargs console=ttySIF0,${baudrate}\0" \
		"fatenv=setenv fileaddr a0000000; fatload mmc 0:1 ${fileaddr} uEnv.txt;" \
			"env import -t ${fileaddr} ${filesize}"

#define CONFIG_SYS_MAX_FLASH_SECT	0
#define CONFIG_SYS_MAX_FLASH_BANKS 0
#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM_0
#define CONFIG_SYS_MEMTEST_END		(PHYS_SDRAM_0 + PHYS_SDRAM_0_SIZE)

#define CONFIG_ENV_SIZE		0x2000


#define CONFIG_SYS_BAUDRATE_TABLE {9600,19200,38400,57600,115200,230400,460800,921600}
#define CONFIG_SYS_LOADS_BAUD_CHANGE 1		/* allow baudrate change */

/* 1MHz RTC clock, SiFive FU540-C000 Manual, section 7 */
#define CONFIG_SYS_HZ_CLOCK	1000000

#endif /* __CONFIG_H */
