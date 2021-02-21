/***************************************************************************
 * Name:
 *     smi712.h
 * License:
 *     2003-2007, Copyright by BLX IC Design Co., Ltd.
 * Description:
 *     Smi 712 VGA card bios driver for BLX 1A
 *     The code from linux framebuffer drivers.
 *
 ***************************************************************************/

#ifndef  __SMI_712_
#define  __SMI_712_
#if 0
#include <config.h>
#include <io.h>
#endif
#if 1   //xuhua
#include <linux/io.h>
#include <linux/types.h>
//#include <ctype.h>
#endif 
#include <string.h>


//#define  IO_BASE  0xbfd00000
#define  IO_BASE  BONITO_PCIIO_BASE_VA
////#ifdef   CONFIG_CPU_GS32I
////#define  IO_BASE  0x50000000
////#else
//#error no #define IO_BASE
////#endif

static  char   *SMIRegs;        /* point to virtual Memory Map IO starting address */
static  char   *SMILFB;         /* point to virtual video memory starting address  */
static  struct par_info hw;     /* used to record hardware information             */
#ifndef writeb
#define writeb(addr, data)      (*(volatile unsigned char *)(addr) = (data))
#endif
#ifndef writew
#define writew(addr, data)      (*(volatile unsigned short *)(addr) = (data))
#endif
#ifndef writel
#define writel(addr, data)      (*(volatile unsigned long *)(addr) = (data))
#endif

#define smi_mmiowb(dat,reg)     writeb(SMIRegs + (reg), (dat))
#define smi_mmioww(dat,reg)     writew(SMIRegs + (reg), (dat))
#define smi_mmiowl(dat,reg)     writel(SMIRegs + (reg), (dat))

#define smi_mmiorb(reg)         readb((unsigned long)(SMIRegs + (reg)))
#define smi_mmiorw(reg)         readw((unsigned long)(SMIRegs + (reg)))
#define smi_mmiorl(reg)         readl((unsigned long)(SMIRegs + (reg)))

#define SIZE_SR00_SR04          (0x04 - 0x00 + 1)
#define SIZE_SR10_SR24          (0x24 - 0x10 + 1)
#define SIZE_SR30_SR75          (0x75 - 0x30 + 1)
#define SIZE_SR80_SR93          (0x93 - 0x80 + 1)
#define SIZE_SRA0_SRAF          (0xAF - 0xA0 + 1)
#define SIZE_GR00_GR08          (0x08 - 0x00 + 1)
#define SIZE_AR00_AR14          (0x14 - 0x00 + 1)
#define SIZE_CR00_CR18          (0x18 - 0x00 + 1)
#define SIZE_CR30_CR4D          (0x4D - 0x30 + 1)
#define SIZE_CR90_CRA7          (0xA7 - 0x90 + 1)
#define SIZE_VPR                (0x6C + 1)
#define SIZE_DPR                (0x44 + 1)

#define numChipIDs              3

#define NR_PALETTE              256
#define NR_RGB                  2

/****************************************************************************
 * Minimum X and Y resolutions
 ****************************************************************************/
#define MIN_XRES                640
#define MIN_YRES                480

static inline void smi_crtcw(int reg, int val)
{
        smi_mmiowb(reg, 0x3d4);
        smi_mmiowb(val, 0x3d5);
}

static inline unsigned int smi_crtcr(int reg)
{       
        smi_mmiowb(reg, 0x3d4);
        return smi_mmiorb(0x3d5);
}

static inline void smi_grphw(int reg, int val)
{
        smi_mmiowb(reg, 0x3ce);
        smi_mmiowb(val, 0x3cf);
}

static inline unsigned int smi_grphr(int reg)
{
        smi_mmiowb(reg, 0x3ce);
        return smi_mmiorb(0x3cf);
}

static inline void smi_attrw(int reg, int val)
{
		smi_mmiorb(0x3da);
		smi_mmiowb(reg, 0x3c0);
        smi_mmiorb(0x3c1);
        smi_mmiowb(val, 0x3c0);
}

static inline void smi_seqw(int reg, int val)
{
        smi_mmiowb(reg, 0x3c4);
        smi_mmiowb(val, 0x3c5);
}

static inline unsigned int smi_seqr(int reg)
{
        smi_mmiowb(reg, 0x3c4);
        return smi_mmiorb(0x3c5);
}
 

struct par_info {
        /* Hardware */
        u16             chipID;
        char            *m_pLFB;
        char            *m_pMMIO;
        char            *m_pDPR;
        char            *m_pVPR;

        u32             width;
        u32             height;
        u32             hz;
        /* glame add */
        u32             bits_per_pixel;
};

/* The next structure holds all information relevant for a specific video mode. */
struct ModeInit
{
        int             mmSizeX;
        int             mmSizeY;
        int             bpp;
        int             hz;
        unsigned char   Init_MISC;
        unsigned char   Init_SR00_SR04[SIZE_SR00_SR04];
        unsigned char   Init_SR10_SR24[SIZE_SR10_SR24];
        unsigned char   Init_SR30_SR75[SIZE_SR30_SR75];
        unsigned char   Init_SR80_SR93[SIZE_SR80_SR93];
        unsigned char   Init_SRA0_SRAF[SIZE_SRA0_SRAF];
        unsigned char   Init_GR00_GR08[SIZE_GR00_GR08];
        unsigned char   Init_AR00_AR14[SIZE_AR00_AR14];
        unsigned char   Init_CR00_CR18[SIZE_CR00_CR18];
        unsigned char   Init_CR30_CR4D[SIZE_CR30_CR4D];
        unsigned char   Init_CR90_CRA7[SIZE_CR90_CRA7];
};


struct ModeInit VGAMode[] =
{
	{
		//  mode#0: 640 x 480  8Bpp  60Hz 
		640, 480, 8, 60,
		//  Init_MISC 
		0xE3,
		{   // Init_SR0_SR4  
			0x03, 0x01, 0x0F, 0x00, 0x0E,
		},
		{  // Init_SR10_SR24  
			0xFF, 0xBE, 0xEF, 0xFF, 0x00, 0x0E, 0x17, 0x2C,
			0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xC4, 0x30, 0x02, 0x01, 0x01,
		},
		{  //  Init_SR30_SR75  
			0x32, 0x03, 0xA0, 0x09, 0xC0, 0x32, 0x32, 0x32,
			0x32, 0x32, 0x32, 0x32, 0x00, 0x00, 0x03, 0xFF,
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC,
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x32, 0x32, 0x32,
			0x04, 0x24, 0x63, 0x4F, 0x52, 0x0B, 0xDF, 0xEA,
			0x04, 0x50, 0x19, 0x32, 0x32, 0x00, 0x00, 0x32,
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00,
			0x50, 0x03, 0x74, 0x14, 0x07, 0x82, 0x07, 0x04,
			0x00, 0x45, 0x30, 0x30, 0x40, 0x30,
		},
		{   //  Init_SR80_SR93 
			0xFF, 0x07, 0x00, 0x6F, 0x7F, 0x7F, 0xFF, 0x32,
			0xF7, 0x00, 0x00, 0x00, 0xEF, 0xFF, 0x32, 0x32,
			0x00, 0x00, 0x00, 0x00,
		},
		{  //  Init_SRA0_SRAF  
			0x00, 0xFF, 0xBF, 0xFF, 0xFF, 0xED, 0xED, 0xED,
			0x7B, 0xFF, 0xFF, 0xFF, 0xBF, 0xEF, 0xFF, 0xDF,
		},
		{   //  Init_GR00_GR08  
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		},
		{   //  Init_AR00_AR14  
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00, 0x00,
		},
		{   //  Init_CR00_CR18  
			0x5F, 0x4F, 0x4F, 0x00, 0x53, 0x1F, 0x0B, 0x3E,
			0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xEA, 0x0C, 0xDF, 0x50, 0x40, 0xDF, 0x00, 0xE3,
			0xFF,
		},
		{   //  Init_CR30_CR4D  
			0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x03, 0x20,
			0x00, 0x00, 0x00, 0x40, 0x00, 0xE7, 0xFF, 0xFD,
			0x5F, 0x4F, 0x00, 0x54, 0x00, 0x0B, 0xDF, 0x00,
			0xEA, 0x0C, 0x2E, 0x00, 0x4F, 0xDF,
		},
		{   // Init_CR90_CRA7  
			0x56, 0xDD, 0x5E, 0xEA, 0x87, 0x44, 0x8F, 0x55,
			0x0A, 0x8F, 0x55, 0x0A, 0x00, 0x00, 0x18, 0x00,
			0x11, 0x10, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A, 0x00,
		},
	},
	
	{
		/*  mode#1: 640 x 480  16Bpp  60Hz */
		640, 480, 16, 60,
		/*  Init_MISC */
		0xE3,
		{   /*  Init_SR0_SR4 */
			0x03, 0x01, 0x0F, 0x00, 0x0E,
		},
		{   /*  Init_SR10_SR24 */
			0xFF, 0xBE, 0xEF, 0xFF, 0x00, 0x0E, 0x17, 0x2C,
			0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xC4, 0x30, 0x02, 0x01, 0x01,
		},
		{   /*  Init_SR30_SR75 */
			0x32, 0x03, 0xA0, 0x09, 0xC0, 0x32, 0x32, 0x32,
			0x32, 0x32, 0x32, 0x32, 0x00, 0x00, 0x03, 0xFF,
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC,
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x32, 0x32, 0x32,
			0x04, 0x24, 0x63, 0x4F, 0x52, 0x0B, 0xDF, 0xEA,
			0x04, 0x50, 0x19, 0x32, 0x32, 0x00, 0x00, 0x32,
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00,
			0x50, 0x03, 0x74, 0x14, 0x07, 0x82, 0x07, 0x04,
			0x00, 0x45, 0x30, 0x30, 0x40, 0x30,
		},
		{   /*  Init_SR80_SR93 */
			0xFF, 0x07, 0x00, 0x6F, 0x7F, 0x7F, 0xFF, 0x32,
			0xF7, 0x00, 0x00, 0x00, 0xEF, 0xFF, 0x32, 0x32,
			0x00, 0x00, 0x00, 0x00,
		},
		{   /*  Init_SRA0_SRAF */
			0x00, 0xFF, 0xBF, 0xFF, 0xFF, 0xED, 0xED, 0xED,
			0x7B, 0xFF, 0xFF, 0xFF, 0xBF, 0xEF, 0xFF, 0xDF,
		},
		{   /*  Init_GR00_GR08 */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		},
		{   /*  Init_AR00_AR14 */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00, 0x00,
		},
		{   /*  Init_CR00_CR18 */
			0x5F, 0x4F, 0x4F, 0x00, 0x53, 0x1F, 0x0B, 0x3E,
			0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xEA, 0x0C, 0xDF, 0x50, 0x40, 0xDF, 0x00, 0xE3,
			0xFF,
		},
		{   /*  Init_CR30_CR4D */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x03, 0x20,
			0x00, 0x00, 0x00, 0x40, 0x00, 0xE7, 0xFF, 0xFD,
			0x5F, 0x4F, 0x00, 0x54, 0x00, 0x0B, 0xDF, 0x00,
			0xEA, 0x0C, 0x2E, 0x00, 0x4F, 0xDF,
		},
		{   /*  Init_CR90_CRA7 */
			0x56, 0xDD, 0x5E, 0xEA, 0x87, 0x44, 0x8F, 0x55,
			0x0A, 0x8F, 0x55, 0x0A, 0x00, 0x00, 0x18, 0x00,
			0x11, 0x10, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A, 0x00,
		},
	},
	{
		/*  mode#2: 640 x 480  24Bpp  60Hz */
		640, 480, 24, 60,
		/*  Init_MISC */
		0xE3,
		{   /*  Init_SR0_SR4 */
			0x03, 0x01, 0x0F, 0x00, 0x0E,
		},
		{   /*  Init_SR10_SR24 */
			0xFF, 0xBE, 0xEF, 0xFF, 0x00, 0x0E, 0x17, 0x2C,
			0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xC4, 0x30, 0x02, 0x01, 0x01,
		},
		{   /*  Init_SR30_SR75 */
			0x32, 0x03, 0xA0, 0x09, 0xC0, 0x32, 0x32, 0x32,
			0x32, 0x32, 0x32, 0x32, 0x00, 0x00, 0x03, 0xFF,
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC,
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x32, 0x32, 0x32,
			0x04, 0x24, 0x63, 0x4F, 0x52, 0x0B, 0xDF, 0xEA,
			0x04, 0x50, 0x19, 0x32, 0x32, 0x00, 0x00, 0x32,
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00,
			0x50, 0x03, 0x74, 0x14, 0x07, 0x82, 0x07, 0x04,
			0x00, 0x45, 0x30, 0x30, 0x40, 0x30,
		},
		{   /*  Init_SR80_SR93 */
			0xFF, 0x07, 0x00, 0x6F, 0x7F, 0x7F, 0xFF, 0x32,
			0xF7, 0x00, 0x00, 0x00, 0xEF, 0xFF, 0x32, 0x32,
			0x00, 0x00, 0x00, 0x00,
		},
		{   /*  Init_SRA0_SRAF */
			0x00, 0xFF, 0xBF, 0xFF, 0xFF, 0xED, 0xED, 0xED,
			0x7B, 0xFF, 0xFF, 0xFF, 0xBF, 0xEF, 0xFF, 0xDF,
		},
		{   /*  Init_GR00_GR08 */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		},
		{   /*  Init_AR00_AR14 */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00, 0x00,
		},
		{   /*  Init_CR00_CR18 */
			0x5F, 0x4F, 0x4F, 0x00, 0x53, 0x1F, 0x0B, 0x3E,
			0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xEA, 0x0C, 0xDF, 0x50, 0x40, 0xDF, 0x00, 0xE3,
			0xFF,
		},
		{   /*  Init_CR30_CR4D */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x03, 0x20,
			0x00, 0x00, 0x00, 0x40, 0x00, 0xE7, 0xFF, 0xFD,
			0x5F, 0x4F, 0x00, 0x54, 0x00, 0x0B, 0xDF, 0x00,
			0xEA, 0x0C, 0x2E, 0x00, 0x4F, 0xDF,
		},
		{   /*  Init_CR90_CRA7 */
			0x56, 0xDD, 0x5E, 0xEA, 0x87, 0x44, 0x8F, 0x55,
			0x0A, 0x8F, 0x55, 0x0A, 0x00, 0x00, 0x18, 0x00,
			0x11, 0x10, 0x0B, 0x0A, 0x0A, 0x0A, 0x0A, 0x00,
		},
	},
	{//  mode#3: 800 x 600  8Bpp  60Hz 
		800,600,8,60,
		0x2B,	// Init_MISC 
		{       //  Init_SR0_SR4 
			0x03, 0x01, 0x0F, 0x03, 0x0E,
		},
		{   // Init_SR10_SR24 
			0xFF, 0xBE, 0xEE, 0xFF, 0x00, 0x0E, 0x17, 0x2C,
			0x99, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xC4, 0x30, 0x02, 0x01, 0x01,
 		},
		{   //  Init_SR30_SR75 
			0x34, 0x03, 0x20, 0x09, 0xC0, 0x24, 0x24, 0x24,
			0x24, 0x24, 0x24, 0x24, 0x00, 0x00, 0x03, 0xFF,
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x38, 0x00, 0xFC,
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x24, 0x24, 0x24,
			0x04, 0x48, 0x83, 0x63, 0x68, 0x72, 0x57, 0x58,
			0x04, 0x55, 0x59, 0x24, 0x24, 0x00, 0x00, 0x24,
			0x01, 0x80, 0x7A, 0x1A, 0x1A, 0x00, 0x00, 0x00,
			0x50, 0x03, 0x74, 0x14, 0x1C, 0x85, 0x35, 0x13,
			0x02, 0x45, 0x30, 0x35, 0x40, 0x20,
		},
		{   // Init_SR80_SR93 
			0xFF, 0x00, 0x00, 0x6F, 0x7F, 0x7F, 0xFF, 0x24,
			0x90, 0x01, 0x2C, 0x01, 0xFF, 0x00, 0x24, 0x24,
			0x00, 0x00, 0x00, 0x00,
		},
		{   //  Init_SRA0_SRAF 
			0x00, 0xFF, 0xBF, 0xFF, 0xFF, 0xED, 0xED, 0xED,
			0x7B, 0xFF, 0xFF, 0xFF, 0xBF, 0xEF, 0xBF, 0xDF,
		},
		{   // Init_GR00_GR08 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		},
		{   //  Init_AR00_AR14 
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00, 0x00,
		},
		{   //  Init_CR00_CR18 
			0x7F, 0x63, 0x63, 0x00, 0x68, 0x18, 0x72, 0xF0,
			0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x58, 0x0C, 0x57, 0x64, 0x40, 0x57, 0x00, 0xE3,
			0xFF,
 		},
		{   //  Init_CR30_CR4D 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x03, 0x20,
			0x00, 0x00, 0x00, 0x40, 0x00, 0xE7, 0xBF, 0xFD,
			0x7F, 0x63, 0x00, 0x69, 0x18, 0x72, 0x57, 0x00,
			0x58, 0x0C, 0xE0, 0x20, 0x63, 0x57,
		},
		{   //  Init_CR90_CRA7 
			0x56, 0x4B, 0x5E, 0x55, 0x86, 0x9D, 0x8E, 0xAA,
			0xDB, 0x2A, 0xDF, 0x33, 0x00, 0x00, 0x18, 0x00,
			0x20, 0x1F, 0x1A, 0x19, 0x0F, 0x0F, 0x0F, 0x00,
		},
	},


	{/*  mode#4: 800 x 600  16Bpp  60Hz */
		800, 600, 16, 60,
		/*  Init_MISC */
		0x2B,
		{   /*  Init_SR0_SR4 */
			0x03, 0x01, 0x0F, 0x03, 0x0E,
		},
		{   /*  Init_SR10_SR24 */
			0xFF, 0xBE, 0xEE, 0xFF, 0x00, 0x0E, 0x17, 0x2C,
			0x99, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xC4, 0x30, 0x02, 0x01, 0x01,
		},
		{   /*  Init_SR30_SR75 */
			0x34, 0x03, 0x20, 0x09, 0xC0, 0x24, 0x24, 0x24,
			0x24, 0x24, 0x24, 0x24, 0x00, 0x00, 0x03, 0xFF,
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x38, 0x00, 0xFC,
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x24, 0x24, 0x24,
			0x04, 0x48, 0x83, 0x63, 0x68, 0x72, 0x57, 0x58,
			0x04, 0x55, 0x59, 0x24, 0x24, 0x00, 0x00, 0x24,
			0x01, 0x80, 0x7A, 0x1A, 0x1A, 0x00, 0x00, 0x00,
			0x50, 0x03, 0x74, 0x14, 0x1C, 0x85, 0x35, 0x13,
			0x02, 0x45, 0x30, 0x35, 0x40, 0x20,
		},
		{   /*  Init_SR80_SR93 */
			0x00, 0x00, 0x00, 0x6F, 0x7F, 0x7F, 0xFF, 0x24,
			0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x24, 0x24,
			0x00, 0x00, 0x00, 0x00,
		},
		{   /*  Init_SRA0_SRAF */
			0x00, 0xFF, 0xBF, 0xFF, 0xFF, 0xED, 0xED, 0xED,
			0x7B, 0xFF, 0xFF, 0xFF, 0xBF, 0xEF, 0xBF, 0xDF,
		},
		{   /*  Init_GR00_GR08 */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		},
		{   /*  Init_AR00_AR14 */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00, 0x00,
		},
		{   /*  Init_CR00_CR18 */
			0x7F, 0x63, 0x63, 0x00, 0x68, 0x18, 0x72, 0xF0,
			0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x58, 0x0C, 0x57, 0x64, 0x40, 0x57, 0x00, 0xE3,
			0xFF,
 		},
		{   /*  Init_CR30_CR4D */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x03, 0x20,
			0x00, 0x00, 0x00, 0x40, 0x00, 0xE7, 0xBF, 0xFD,
			0x7F, 0x63, 0x00, 0x69, 0x18, 0x72, 0x57, 0x00,
			0x58, 0x0C, 0xE0, 0x20, 0x63, 0x57,
		},
		{   /*  Init_CR90_CRA7 */
			0x56, 0x4B, 0x5E, 0x55, 0x86, 0x9D, 0x8E, 0xAA,
			0xDB, 0x2A, 0xDF, 0x33, 0x00, 0x00, 0x18, 0x00,
			0x20, 0x1F, 0x1A, 0x19, 0x0F, 0x0F, 0x0F, 0x00,
		},
	},
		{/*  mode#5: 800 x 600  24Bpp  60Hz */
		800,600,24,60,
		0x2B,
		{   /*  Init_SR0_SR4 */
			0x03, 0x01, 0x0F, 0x03, 0x0E,
		},
		{   /*  Init_SR10_SR24 */
			0xFF, 0xBE, 0xEE, 0xFF, 0x00, 0x0E, 0x17, 0x2C,
			0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0xC4, 0x30, 0x02, 0x01, 0x01,
		},
		{   /*  Init_SR30_SR75 */
			0x36, 0x03, 0x20, 0x09, 0xC0, 0x36, 0x36, 0x36,
			0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x03, 0xFF,
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC,
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x36, 0x36, 0x36,
			0x04, 0x48, 0x83, 0x63, 0x68, 0x72, 0x57, 0x58,
			0x04, 0x55, 0x59, 0x36, 0x36, 0x00, 0x00, 0x36,
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00,
			0x50, 0x03, 0x74, 0x14, 0x1C, 0x85, 0x35, 0x13,
			0x02, 0x45, 0x30, 0x30, 0x40, 0x20,
		},
		{   /*  Init_SR80_SR93 */
			0xFF, 0x07, 0x00, 0x6F, 0x7F, 0x7F, 0xFF, 0x36,
			0xF7, 0x00, 0x00, 0x00, 0xEF, 0xFF, 0x36, 0x36,
			0x00, 0x00, 0x00, 0x00,
		},
		{   /*  Init_SRA0_SRAF */
			0x00, 0xFF, 0xBF, 0xFF, 0xFF, 0xED, 0xED, 0xED,
			0x7B, 0xFF, 0xFF, 0xFF, 0xBF, 0xEF, 0xBF, 0xDF,
		},
		{   /*  Init_GR00_GR08 */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
			0xFF,
		},
		{   /*  Init_AR00_AR14 */
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x41, 0x00, 0x0F, 0x00, 0x00,
		},
		{   /*  Init_CR00_CR18 */
			0x7F, 0x63, 0x63, 0x00, 0x68, 0x18, 0x72, 0xF0,
			0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x58, 0x0C, 0x57, 0x64, 0x40, 0x57, 0x00, 0xE3,
			0xFF,
		},
		{   /*  Init_CR30_CR4D */
			0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x03, 0x20,
			0x00, 0x00, 0x00, 0x40, 0x00, 0xE7, 0xBF, 0xFD,
			0x7F, 0x63, 0x00, 0x69, 0x18, 0x72, 0x57, 0x00,
			0x58, 0x0C, 0xE0, 0x20, 0x63, 0x57,
		},
		{   /*  Init_CR90_CRA7 */
			0x56, 0x4B, 0x5E, 0x55, 0x86, 0x9D, 0x8E, 0xAA,
			0xDB, 0x2A, 0xDF, 0x33, 0x00, 0x00, 0x18, 0x00,
			0x20, 0x1F, 0x1A, 0x19, 0x0F, 0x0F, 0x0F, 0x00,
		},
	},
    {//  mode#6: 1024 x 768  8Bpp  60Hz
	 	1024,768,8,60,
		// Init_MISC                                    
		0xEB,                                               
		{   //  Init_SR0_SR4                             
			0x03, 0x01, 0x0F, 0x03, 0x0E,                   
		},                                                  
		{   //  Init_SR10_SR24                            
			0xF3, 0xB6, 0xC0, 0xDD, 0x00, 0x0E, 0x17, 0x2C, 
			0x99, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0xC4, 0x30, 0x02, 0x01, 0x01,                   
		},                                                  
		{   // Init_SR30_SR75                         
			0x38, 0x03, 0x20, 0x09, 0xC0, 0x3A, 0x3A, 0x3A, 
			0x3A, 0x3A, 0x3A, 0x3A, 0x00, 0x00, 0x03, 0xFF, 
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC, 
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x00, 0x00, 0x3A, 
			0x06, 0x68, 0xA7, 0x7F, 0x83, 0x24, 0xFF, 0x03, 
			0x00, 0x60, 0x59, 0x3A, 0x3A, 0x00, 0x00, 0x3A, 
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00, 
			0x50, 0x03, 0x74, 0x14, 0x3B, 0x0D, 0x09, 0x02, 
			0x04, 0x45, 0x30, 0x30, 0x40, 0x20,             
		},                                                 
		{   //  Init_SR80_SR93                          
			0xFF, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3A, 
			0xF7, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x3A, 0x3A, 
			0x00, 0x00, 0x00, 0x00,                         
		},                                                  
		{   //  Init_SRA0_SRAF                          
			0x00, 0xFB, 0x9F, 0x01, 0x00, 0xED, 0xED, 0xED, 
			0x7B, 0xFB, 0xFF, 0xFF, 0x97, 0xEF, 0xBF, 0xDF, 
		},                                                  
		{   //  Init_GR00_GR08                           
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 
			0xFF,                                           
		},                                                  
		{   //  Init_AR00_AR14                           
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
			0x41, 0x00, 0x0F, 0x00, 0x00,                   
		},                                                  
		{   //  Init_CR00_CR18                            
			0xA3, 0x7F, 0x7F, 0x00, 0x85, 0x16, 0x24, 0xF5, 
			0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x03, 0x09, 0xFF, 0x80, 0x40, 0xFF, 0x00, 0xE3, 
			0xFF,                                           
		},                                                  
		{   //  Init_CR30_CR4D                           
			0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x20, 
			0x00, 0x00, 0x00, 0x40, 0x00, 0xFF, 0xBF, 0xFF, 
			0xA3, 0x7F, 0x00, 0x86, 0x15, 0x24, 0xFF, 0x00, 
			0x01, 0x07, 0xE5, 0x20, 0x7F, 0xFF,             
		},                                                  
		{   // Init_CR90_CRA7                            
			0x55, 0xD9, 0x5D, 0xE1, 0x86, 0x1B, 0x8E, 0x26, 
			0xDA, 0x8D, 0xDE, 0x94, 0x00, 0x00, 0x18, 0x00, 
			0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x15, 0x03, 
		},                                                  
	},                                                      
    { /*  mode#7: 1024 x 768  16Bpp  60Hz */ 
        1024,768,16,60,
		/*  Init_MISC */                                    
		0xEB,                                               
		{   /*  Init_SR0_SR4 */                             
			0x03, 0x01, 0x0F, 0x03, 0x0E,                   
		},                                                  
		{   /*  Init_SR10_SR24 */                           
			0xF3, 0xB6, 0xC0, 0xDD, 0x00, 0x0E, 0x17, 0x2C, 
			0x99, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0xC4, 0x32, 0x02, 0x01, 0x01,                   
		},                                                  
		{   /*  Init_SR30_SR75 */                           
			0x38, 0x03, 0x20, 0x09, 0xC0, 0x3A, 0x3A, 0x3A, 
			0x3A, 0x3A, 0x3A, 0x3A, 0x00, 0x00, 0x03, 0xFF, 
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC, 
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x00, 0x00, 0x3A, 
			0x06, 0x68, 0xA7, 0x7F, 0x83, 0x24, 0xFF, 0x03, 
			0x00, 0x60, 0x59, 0x3A, 0x3A, 0x00, 0x00, 0x3A, 
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00, 
			0x50, 0x03, 0x74, 0x14, 0x3B, 0x0D, 0x09, 0x02, 
			0x04, 0x45, 0x30, 0x30, 0x40, 0x20,             
		},                                                  
		{   /*  Init_SR80_SR93 */                           
			0xFF, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3A, 
			0xF7, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x3A, 0x3A, 
			0x00, 0x00, 0x00, 0x00,                         
		},                                                  
		{   /*  Init_SRA0_SRAF */                           
			0x00, 0xFB, 0x9F, 0x01, 0x00, 0xED, 0xED, 0xED, 
			0x7B, 0xFB, 0xFF, 0xFF, 0x97, 0xEF, 0xBF, 0xDF, 
		},                                                  
		{   /*  Init_GR00_GR08 */                           
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 
			0xFF,                                           
		},                                                  
		{   /*  Init_AR00_AR14 */                           
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
			0x41, 0x00, 0x0F, 0x00, 0x00,                   
		},                                                  
		{   /*  Init_CR00_CR18 */                           
			0xA3, 0x7F, 0x7F, 0x00, 0x85, 0x16, 0x24, 0xF5, 
			0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x03, 0x09, 0xFF, 0x80, 0x40, 0xFF, 0x00, 0xE3, 
			0xFF,                                           
		},                                                  
		{   /*  Init_CR30_CR4D */                           
			0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x20, 
			0x00, 0x00, 0x00, 0x40, 0x00, 0xFF, 0xBF, 0xFF, 
			0xA3, 0x7F, 0x00, 0x86, 0x15, 0x24, 0xFF, 0x00, 
			0x01, 0x07, 0xE5, 0x20, 0x7F, 0xFF,             
		},                                                  
		{   /*  Init_CR90_CRA7 */                           
			0x55, 0xD9, 0x5D, 0xE1, 0x86, 0x1B, 0x8E, 0x26, 
			0xDA, 0x8D, 0xDE, 0x94, 0x00, 0x00, 0x18, 0x00, 
			0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x15, 0x03, 
		},                                                  
	},                                                      
	{  /*  mode#8: 1024 x 768  24Bpp  60Hz */               
        1024,768,24,60,
			/*  Init_MISC */                                
		0xEB,                                               
		{   /*  Init_SR0_SR4 */                             
			0x03, 0x01, 0x0F, 0x03, 0x0E,                   
		},                                                  
		{   /*  Init_SR10_SR24 */                           
			0xF3, 0xB6, 0xC0, 0xDD, 0x00, 0x0E, 0x17, 0x2C, 
			0x99, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0xC4, 0x30, 0x02, 0x01, 0x01,                   
		},                                                  
		{   /*  Init_SR30_SR75 */                           
			0x38, 0x03, 0x20, 0x09, 0xC0, 0x3A, 0x3A, 0x3A, 
			0x3A, 0x3A, 0x3A, 0x3A, 0x00, 0x00, 0x03, 0xFF, 
			0x00, 0xFC, 0x00, 0x00, 0x20, 0x18, 0x00, 0xFC, 
			0x20, 0x0C, 0x44, 0x20, 0x00, 0x00, 0x00, 0x3A, 
			0x06, 0x68, 0xA7, 0x7F, 0x83, 0x24, 0xFF, 0x03, 
			0x00, 0x60, 0x59, 0x3A, 0x3A, 0x00, 0x00, 0x3A, 
			0x01, 0x80, 0x7E, 0x1A, 0x1A, 0x00, 0x00, 0x00, 
			0x50, 0x03, 0x74, 0x14, 0x3B, 0x0D, 0x09, 0x02, 
			0x04, 0x45, 0x30, 0x30, 0x40, 0x20,             
		},                                                  
		{   /*  Init_SR80_SR93 */                           
			0xFF, 0x07, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x3A, 
			0xF7, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x3A, 0x3A, 
			0x00, 0x00, 0x00, 0x00,                         
		},                                                  
		{   /*  Init_SRA0_SRAF */                           
			0x00, 0xFB, 0x9F, 0x01, 0x00, 0xED, 0xED, 0xED, 
			0x7B, 0xFB, 0xFF, 0xFF, 0x97, 0xEF, 0xBF, 0xDF, 
		},                                                  
		{   /*  Init_GR00_GR08 */                           
			0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 
			0xFF,                                           
		},                                                  
		{   /*  Init_AR00_AR14 */                           
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
			0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
			0x41, 0x00, 0x0F, 0x00, 0x00,                   
		},                                                  
		{   /*  Init_CR00_CR18 */                           
			0xA3, 0x7F, 0x7F, 0x00, 0x85, 0x16, 0x24, 0xF5, 
			0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x03, 0x09, 0xFF, 0x80, 0x40, 0xFF, 0x00, 0xE3, 
			0xFF,                                           
		},                                                  
		{   /*  Init_CR30_CR4D */                           
			0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x20, 
			0x00, 0x00, 0x00, 0x40, 0x00, 0xFF, 0xBF, 0xFF, 
			0xA3, 0x7F, 0x00, 0x86, 0x15, 0x24, 0xFF, 0x00, 
			0x01, 0x07, 0xE5, 0x20, 0x7F, 0xFF,             
		},                                                  
		{   /*  Init_CR90_CRA7 */                           
			0x55, 0xD9, 0x5D, 0xE1, 0x86, 0x1B, 0x8E, 0x26, 
			0xDA, 0x8D, 0xDE, 0x94, 0x00, 0x00, 0x18, 0x00, 
			0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x15, 0x03, 
		},                                                  
	},														

};

#define numVGAModes             sizeof(VGAMode)/sizeof(VGAMode[0])



#endif /*__SMI_712__*/

/* ---------------------------------------------------------------------- */
// $Log$