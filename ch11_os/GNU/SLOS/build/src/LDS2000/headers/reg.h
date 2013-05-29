
#ifndef SMART_REG_H
#define SMART_REG_H

#define	__REG	*(volatile unsigned long *)
#define	__REG32	*(volatile unsigned long *)
#define	__REG16	*(volatile unsigned short *)
#define	__REG8	*(volatile unsigned char *)

#define BOOT_SWI 0xa3fe0024
#define BOOT_IRQ 0xa3fe0034
#define	TICKS_PER_SECOND 32768
#define	RCNR	 __REG(0x40900000)

//Interrupt Control Registers
//#define	INTERRUPT_CONTROL_BASE 0x40D00000
#define		INTER_BASE 0x40D00000
#define	ICIP		0x00
#define	ICMR		0x04
#define	ICLR		0x08
#define	ICFP		0x0C
#define	ICPR		0x10
#define	ICCR		0x14

#define	CLOCK_MANAGER_BASE 0x41300000

#define	CCCR		0x00
#define	CKEN		0x04
#define	OSCC		0x08
//#define	OSCR		0x40A00010

//OS Timer base
#define TIMER_BASE 0x40A00000

#define OSMR0		0x00
#define OSMR1		0x04
#define OSMR2		0x08
#define OSMR3		0x0C
#define OSCR		0x10
#define OSSR		0x14
#define OIER		0x1C

//GPIO registers
#define	GPIO_BASE	0x40E00000

#define	GPLR0		0x00
#define	GPLR1		0x04
#define	GPLR2		0x08
#define	GPDR0		0x0C
#define	GPDR1		0x10
#define	GPDR2		0x14
#define	GPSR0		0x18
#define	GPSR1		0x1C
#define	GPSR2		0x20
#define	GPCR0		0x24
#define	GPCR1		0x28
#define	GPCR2		0x2C
#define	GRER0		0x30
#define	GRER1		0x34
#define	GRER2		0x38
#define	GFER0		0x3C
#define	GFER1		0x40
#define	GFER2		0x44
#define	GDER0		0x48
#define	GDER1		0x4C
#define	GDER2		0x50
#define	GAFR0_L	0x54
#define	GAFR0_U	0x58
#define	GAFR1_L	0x5C
#define	GAFR1_U	0x60
#define	GAFR2_L	0x64
#define	GAFR2_U	0x68

//Memory Control Registers
#define	MEM_CTL_BASE      0x48000000

#define	MDCNFG	0x00
#define	MDREFR	0x04
#define	MSC0		0x08
#define	MSC1		0x0C
#define	MSC2		0x10
#define	MECR		0x14
#define	SXCNFG	0x1C
#define	MCMEM0	0x28
#define	MCMEM1	0x2C
#define	MCATT0	0x30
#define	MCATT1	0x34
#define	MCIO0		0x38
#define	MCIO1		0x3C
#define	MDMRS		0x40

// Power management
#define	RCSR		0x40F00030
#define	RCSR_SLEEP	0x00000004
#define	PSPR		0x40F00008
#define	PSSR		0x40F00004
//#define	PSSR_PH	0x00000010
//#define	PSSR_RDH	0x00000020
//#define	PSSR_STATUS_MASK  0x00000007

//FFUART Registers
#define	FFUART_BASE       0x40100000

#define	FFRBR		0x00
#define	FFTHR		0x00
#define	FFIER		0x04
#define	FFIIR		0x08
#define	FFFCR		0x08
#define	FFLCR		0x0C
#define	FFMCR		0x10
#define	FFLSR		0x14
#define	FFMSR		0x18
#define	FFSPR		0x1C
#define	FFISR		0x20
#define	FFDLL		0x00
#define	FFDLH		0x04

// Status bits.
#define	STATUS_BUSY			0x00800080
#define	STATUS_ERR_FILTER		0x007E007E
#define	STATUS_LOCK_DETECT		0x00020002
#define	STATUS_VOLT_RANG_ERR		0x00080008
#define	STATUS_CMD_SEQ_ERR		0x00300030
#define	STATUS_PGM_ERR			0x00100010
#define	STATUS_LOCK_ERR			0x00100010
#define	STATUS_ERASE_ERR		0x00200020
#define	STATUS_UNLOCK_ERR		0x00200020
#define	STATUS_LOCK_ERR_FILTER	0x00380038

#endif
