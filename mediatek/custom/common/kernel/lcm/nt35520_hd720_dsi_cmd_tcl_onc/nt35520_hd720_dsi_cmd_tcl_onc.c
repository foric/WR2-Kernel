#ifndef BUILD_LK
#include <linux/string.h>
#endif

#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
// Local Constants
// ---------------------------------------------------------------------------
//TN801031 added 2013.06.17
//#define TN_DISPLAY 1
//#define TN_DISPLAY 0

#if TN_DISPLAY
	#define FRAME_WIDTH  (720)
	#define FRAME_HEIGHT (1280)
#else
	#define FRAME_WIDTH  (540)
	#define FRAME_HEIGHT (960)
#endif
//TN801031 added 2013.06.17

#define LCM_DSI_CMD_MODE    1
#define LCM_ID (0x69)

#define REGFLAG_DELAY 0xAB
#define REGFLAG_END_OF_TABLE 0xAA // END OF REGISTERS MARKER

#define LCM_ID1 0x00
#define LCM_ID2 0x00
#define LCM_ID3 0x00

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
#define NT35520    1

//#define LCM_DSI_CMD_MODE									1

// ---------------------------------------------------------------------------
// Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v) (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
// Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd) lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums) lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd) lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size) lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size) 

struct LCM_setting_table {
unsigned char cmd;
unsigned char count;
unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
{0xFF,4,{0xAA,0x55,0xA5,0x80}},
{0x6F,1,{0x13}},
{0xF7,1,{0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x06}},

//tn_display
{0x35,1,{0x00}},    //TE enable
{0x44,2,{0x00,0x40}}, //TE delay
//tn_display

{0xB0,2,{0x00,0x10}},
{0xB1,2,{0x12,0x14}},
{0xB2,2,{0x16,0x18}},
{0xB3,2,{0x1A,0x2A}},
{0xB4,2,{0x29,0x02}},
{0xB5,2,{0x34,0x31}},
{0xB6,2,{0x34,0x31}},
{0xB7,2,{0x31,0x34}},
{0xB8,2,{0x34,0x04}},
{0xB9,2,{0x34,0x34}},
//TN_DISPLAY
//{0xBA,2,{0x34,0x34}},
//{0xBB,2,{0x05,0x34}},
{0xBA,2,{0x31, 0x31}},
{0xBB,2,{0x31, 0x08}},
//TN_DISPLAY
{0xBC,2,{0x31,0x31}},
{0xBD,2,{0x31,0x34}},
{0xBE,2,{0x31,0x34}},
{0xBF,2,{0x03,0x29}},
{0xC0,2,{0x2A,0x1B}},
{0xC1,2,{0x19,0x17}},
{0xC2,2,{0x15,0x13}},
{0xC3,2,{0x11,0x01}},
{0xE5,2,{0x31,0x31}},
{0xC4,2,{0x03,0x1B}},
{0xC5,2,{0x19,0x17}},
{0xC4,1,{0x82}},
{0xC5,1,{0xA2}},
{0xC6,2,{0x15,0x13}},
{0xC7,2,{0x11,0x2A}},
{0xC8,2,{0x29,0x01}},
{0xC9,2,{0x34,0x31}},
{0xCA,2,{0x34,0x31}},
{0xCB,2,{0x31,0x34}},
{0xCC,2,{0x34,0x05}},
{0xCD,2,{0x34,0x34}},
{0xCE,2,{0x34,0x34}},
{0xCF,2,{0x04,0x34}},
{0xD0,2,{0x31,0x31}},
{0xD1,2,{0x31,0x34}},
{0xD2,2,{0x31,0x34}},
{0xD3,2,{0x00,0x29}},
{0xD4,2,{0x2A,0x10}},
{0xD5,2,{0x12,0x14}},
{0xD6,2,{0x16,0x18}},
{0xD7,2,{0x1A,0x02}},
{0xE5,2,{0x31,0x31}},
{0xE6,2,{0x31,0x31}},
{0xD8,5,{0x00,0x00,0x00,0x14,0x00}},
{0xD9,5,{0x00,0x15,0x00,0x00,0x00}},
{0xE7,1,{0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xED,1,{0x30}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xB1,2,{0x00,0x00}},
{0xB0,2,{0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xE5,1,{0x00}},
{0xFF,4,{0xAA,0x55,0xA5,0x80}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xB0,2,{0x17,0x06}},
{0xB8,1,{0x08}},

{0xBD,5,{0x03,0x07,0x00,0x03,0x00}},

{0xB1,2,{0x17,0x06}},
{0xB9,2,{0x00,0x07}},
{0xB2,2,{0x00,0x00}},
{0xBA,2,{0x00,0x00}},
{0xB3,2,{0x17,0x06}},
{0xBB,2,{0x0A,0x03}},
{0xB4,2,{0x17,0x06}},
{0xB5,2,{0x17,0x06}},
{0xB6,2,{0x14,0x03}},
{0xB7,2,{0x00,0x00}},
{0xBC,2,{0x02,0x00}},
{0xE5,1,{0x06}},
{0xE6,1,{0x06}},
{0xE7,1,{0x00}},
{0xE8,1,{0x06}},
{0xE9,1,{0x06}},
{0xEA,1,{0x06}},
{0xEB,1,{0x00}},
{0xEC,1,{0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xC0,1,{0x07}},
{0xC1,1,{0x80}},
{0xC2,1,{0xA4}},
{0xC3,1,{0x05}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xB2,5,{0x05,0x00,0x00,0x00,0x00}},
{0xB3,5,{0x05,0x00,0x00,0x00,0x00}},
{0xB4,5,{0x05,0x00,0x00,0x00,0x00}},
{0xB3,5,{0x05,0x00,0x17,0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xC4,1,{0x00}},
{0xC5,1,{0x02}},
{0xC6,1,{0x22}},
{0xC7,1,{0x03}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xB6,5,{0x12,0x00,0x19,0x00,0x00}},
{0xB7,5,{0x12,0x00,0x19,0x00,0x00}},
{0xB8,5,{0x12,0x00,0x19,0x00,0x00}},
{0xB9,5,{0x12,0x00,0x19,0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xC8,2,{0x05,0x30}},
{0xC9,2,{0x01,0x31}},
{0xCA,2,{0x03,0x21}},
{0xCB,2,{0x01,0x20}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xBA,5,{0x57,0x00,0x00,0x00,0x00}},
{0xBB,5,{0x57,0x00,0x00,0x00,0x00}},
{0xBC,5,{0x75,0x00,0x1A,0x00,0x00}},
{0xBD,5,{0x53,0x00,0x1A,0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xD1,5,{0x00,0x04,0xFE,0x07,0x10}},
{0xD2,5,{0x10,0x05,0x04,0x03,0x10}},
{0xD3,5,{0x20,0x00,0x48,0x07,0x10}},
{0xD4,5,{0x30,0x00,0x43,0x07,0x10}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xD0,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0xD5,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x6F,1,{0x07}},
{0xD5,4,{0x00,0x00,0x00,0x00}},
{0xD6,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x6F,1,{0x07}},
{0xD6,4,{0x00,0x00,0x00,0x00}},
{0xD7,7,{0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{0x6F,1,{0x07}},
{0xD7,4,{0x00,0x00,0x00,0x00}},
{0xD8,5,{0x00,0x00,0x00,0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x05}},
{0xCC,3,{0x00,0x00,0x3E}},
{0xCD,3,{0x00,0x00,0x3E}},
{0xCE,3,{0x00,0x00,0x02}},
{0xCF,3,{0x00,0x00,0x02}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xC0,4,{0x00,0x34,0x00,0x00}},
{0xC1,4,{0x00,0x34,0x00,0x00}},
{0xC2,4,{0x00,0x34,0x00,0x00}},
{0xC3,4,{0x00,0x34,0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x03}},
{0xC4,1,{0x20}},
{0xC5,1,{0x80}},
{0xC6,1,{0x00}},
{0xC7,1,{0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xB3,2,{0x38,0x38}},
{0xB4,2,{0x11,0x11}},
{0xCA,1,{0x01}},
{0xC4,2,{0x11,0x11}},
{0xB0,2,{0x02,0x02}},
{0xB1,2,{0x02,0x02}},
{0xBC,2,{0xA0,0x00}},
{0xBD,2,{0xA0,0x00}},
{0xB9,2,{0x44,0x44}},
{0xBA,2,{0x24,0x24}},
{0xBE,1,{0x4E}},
{0xC0,1,{0x0C}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
{0xBC,2,{0x00,0x00}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x02}},
{0xEE,1,{0x02}},
{0x6F,1,{0x00}},
{0xB0,6,{0x00,0x9C,0x00,0xB5,0x00,0xDE}},
{0x6F,1,{0x06}},
{0xB0,6,{0x00,0xFB,0x01,0x12,0x01,0x36}},
{0x6F,1,{0x0C}},
{0xB0,4,{0x01,0x52,0x01,0x7C}},
{0x6F,1,{0x00}},
{0xB1,6,{0x01,0x9E,0x01,0xD0,0x01,0xF8}},
{0x6F,1,{0x06}},
{0xB1,6,{0x02,0x32,0x02,0x61,0x02,0x62}},
{0x6F,1,{0x0C}},
{0xB1,4,{0x02,0x8D,0x02,0xBF}},
{0x6F,1,{0x00}},
{0xB2,6,{0x02,0xDA,0x03,0x03,0x03,0x21}},
{0x6F,1,{0x06}},
{0xB2,6,{0x03,0x47,0x03,0x61,0x03,0x80}},
{0x6F,1,{0x0C}},
{0xB2,4,{0x03,0x92,0x03,0xA7}},
{0xB3,4,{0x03,0xBB,0x03,0xC2}},
{0x6F,1,{0x00}},
{0xBC,6,{0x00,0x4B,0x00,0x65,0x00,0x90}},
{0x6F,1,{0x06}},
{0xBC,6,{0x00,0xAE,0x00,0xC7,0x00,0xEE}},
{0x6F,1,{0x0C}},
{0xBC,4,{0x01,0x0D,0x01,0x3E}},
{0x6F,1,{0x00}},
{0xBD,6,{0x01,0x65,0x01,0xA3,0x01,0xD4}},
{0x6F,1,{0x06}},
{0xBD,6,{0x02,0x1E,0x02,0x58,0x02,0x5A}},
{0x6F,1,{0x0C}},
{0xBD,4,{0x02,0x8D,0x02,0xBF}},
{0x6F,1,{0x00}},
{0xBE,6,{0x02,0xE1,0x03,0x0D,0x03,0x2A}},
{0x6F,1,{0x06}},
{0xBE,6,{0x03,0x4F,0x03,0x67,0x03,0x84}},
{0x6F,1,{0x0C}},
{0xBE,4,{0x03,0x95,0x03,0xA9}},
{0x6F,1,{0x00}},
{0xBF,4,{0x03,0xBC,0x03,0xC2}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x00}},
      
{0xB1,2,{0x7A,0x21}},
//tn_display
//{0xBD,5,{0x02,0x67,0x20,0x20,0x00}},    //帧率66hz
{0xBD,5,{0x02,0x10,0x12,0x12,0x00}},//72hz.test tcp
//tn_display

{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xCE,1,{0x66}},
{0xF0,5,{0x55,0xAA,0x52,0x08,0x01}},
{0xB3,2,{0x38,0x38}},
{0xBE,1,{0x4E}},

{0x11,0,{}},
{REGFLAG_DELAY, 150, {}},
{0x29,0,{}},
{0x2C,0,{}},        
        
{REGFLAG_DELAY, 50, {}}, 
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
// Sleep Out
{0x11, 1, {0x00}},
{REGFLAG_DELAY, 120, {}},

// Display ON
{0x29, 1, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
// Display off sequence
{0x28, 1, {0x00}},

// Sleep Mode On
{0x10, 1, {0x00}},

{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_backlight_level_setting[] = {
{0x51, 1, {0xFF}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};


static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

	for(i = 0; i < count; i++) {

		unsigned cmd;
		cmd = table[i].cmd;

		switch (cmd) {
			case REGFLAG_DELAY :
			MDELAY(table[i].count);
			break;

			case REGFLAG_END_OF_TABLE :
			break;

			default:
			dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update); 
		}
	}
}

// ---------------------------------------------------------------------------
// LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

        #if (LCM_DSI_CMD_MODE)
		params->dsi.mode   = CMD_MODE;
        #else
		params->dsi.mode   = BURST_VDO_MODE; //SYNC_PULSE_VDO_MODE;//BURST_VDO_MODE; 
        #endif

		//params->dbi.te_mode = LCM_DBI_TE_MODE_VSYNC_ONLY;		//tinnohd.add test tcp

		params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;
		params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;

		// DSI
		/* Command mode setting */
		//1 Three lane or Four lane
		params->dsi.LANE_NUM				= LCM_FOUR_LANE;			//LCM_THREE_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Video mode setting		
		////params->dsi.intermediat_buffer_num = 0;//because DSI/DPI HW design change, this parameters should be 0 when video mode in MT658X; or memory leakage

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;


		params->dsi.word_count=(FRAME_WIDTH)*3;	

		//params->dsi.HS_PRPR=3;
		//params->dsi.CLK_HS_POST = 26;
		//方案二
		//params->dsi.PLL_CLOCK = LCM_DSI_6589_PLL_CLOCK_325;//LCM_DSI_6589_PLL_CLOCK_260;//LCM_DSI_6589_PLL_CLOCK_286
		//方案一
		//params->dsi.PLL_CLOCK = 250;
		///	params->dsi.LPX		= 80;///bbk add

		// Highly depends on LCD driver capability.
		// Not support in MT6573
		params->dsi.packet_size=256;
		//params->dsi.PLL_CLOCK = 250;//songyutao test

		params->dsi.pll_div1=0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div =17;  //songyutao 17 to 23  // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	
		//songyutao test
		#if 0
		params->dsi.HS_PRPR=3;
		params->dsi.CLK_HS_POST = 26;
		params->dsi.DA_HS_EXIT =35;
		params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2=1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div =19;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	

		params->dsi.noncont_clock = TRUE;
		params->dsi.noncont_clock_period = 2;	// Unit : frames	
		#endif
		//end
}

static void lcm_init(void)
{
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(120);
#if 1
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
#else
	lcm_init_register();
#endif
}


static void lcm_suspend(void)
{
	//push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
		unsigned int data_array[16];
	//return;
	data_array[0] = 0x00280500; 
	dsi_set_cmdq(data_array, 1, 1);
	data_array[0] = 0x00100500; 
	dsi_set_cmdq(data_array, 1, 1);
	#ifndef BUILD_LK
	extern void emmc_get_id_test(void);//songyutao
	//emmc_get_id_test();
	#endif	
}


static void lcm_resume(void)
{
	//lcm_init();
	//push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
		unsigned int data_array[16];


	//return;
	data_array[0] = 0x00110500; 
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(150);
	data_array[0] = 0x00290500; 
	dsi_set_cmdq(data_array, 1, 1);
	MDELAY(20);



}


static void lcm_update(unsigned int x, unsigned int y,
unsigned int width, unsigned int height)
{
#if 0
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;
	
	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

//	data_array[0]= 0x00290508; //HW bug, so need send one HS packet
//	dsi_set_cmdq(data_array, 1, 1);
	
	data_array[0]= 0x002c3909;
	//#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
	//MEDIATEK\platform\mt6589\kernel\drivers\video\dsi_drv.c(2768):void DSI_set_cmdq(unsigned int *pdata, unsigned int queue_size, unsigned char force_update)
	dsi_set_cmdq(data_array, 1, 0);
	#ifdef BUILD_LK
	printf(" lcm_update\n");
	#else
	printk("lcm_update\n");
	#endif
#else
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);
	
	data_array[0]= 0x00053902;
	data_array[1]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[2]= (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

//	data_array[0]= 0x00290508; //HW bug, so need send one HS packet
//	dsi_set_cmdq(data_array, 1, 1);
	
	data_array[0]= 0x002c3909;
	//#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
	//MEDIATEK\platform\mt6589\kernel\drivers\video\dsi_drv.c(2768):void DSI_set_cmdq(unsigned int *pdata, unsigned int queue_size, unsigned char force_update)
	dsi_set_cmdq(data_array, 1, 0);
#endif
}

static void lcm_setbacklight(unsigned int level)
{
// Refresh value of backlight level.
//lcm_backlight_level_setting[0].para_list[0] = level;

//push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
}

extern int IMM_GetOneChannelValue(int dwChannel, int data[4], int* rawdata);
static unsigned int lcm_compare_id(void)
{
#if 0
	unsigned int id=0;
	unsigned char buffer[8];
	unsigned int array[16];  

	 SET_RESET_PIN(1);
	 SET_RESET_PIN(0);
	 MDELAY(10);
	 SET_RESET_PIN(1);
	 MDELAY(10); 
	 
	array[0] = 0x00063902;                          
	array[1] = 0x52AA55F0;      
	array[2] = 0x00000108;            
	dsi_set_cmdq(array, 3, 1);     
	 MDELAY(10);

	 array[0] = 0x00013700;// read id return two byte,version and id
	 dsi_set_cmdq(array, 1, 1);
	 
	//read_reg_v2(0x04, buffer, 3);//if read 0x04,should get 0x008000,that is both OK.
	read_reg_v2(0xc5, buffer,2);
	id = buffer[0]<<8 |buffer[1];
     #ifdef BUILD_LK
		printf("songyutao nt35520 lk id = 0x%x \n", id);
    #else
		printk("songyutao nt35520 k id = 0x%x  0x%x \n", buffer[0],buffer[1]);
    #endif
	
	if(id == 0x55)
		return 1;
	else
		return 0;
#else
	unsigned int id1=0,id2=0;
	unsigned char buffer[2];
	unsigned int array[16];  
	unsigned int data_array[8];
	unsigned int data[4];
	int volt = 0;
	
	#ifdef BUILD_LK
	printf("tcl_oncell lishengli: lcm_compare_id start\n");
	#else
	printk("tcl_oncell lishengli: lcm_compare_id start\n");
	#endif
	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(20); 
	
      data_array[0] = 0x00063902;                          
      data_array[1] = 0x52AA55F0;      
      data_array[2] = 0x00000108;            
      dsi_set_cmdq(data_array, 3, 1);     
	MDELAY(5);

	array[0] = 0x00023700;// read id return two byte,version and id
	dsi_set_cmdq(array, 1, 1);
	
	read_reg_v2(0xc5, buffer, 2);
	id1 = buffer[0]; //we only need ID
	id2 = buffer[1];
    #ifdef BUILD_LK
		printf("tcl_oncell lishengli nt35520 id1 = 0x%08x  id2 = 0x%08x\n", id1,id2);
    #else
		printk("tcl_oncell lishengli nt35520 id1 = 0x%08x  id2 = 0x%08x\n", id1,id2);
    #endif
#ifdef BUILD_LK
	printf("tcl_oncell lishengli: lcm_compare_id end\n");
	#else
	printk("tcl_oncell lishengli: lcm_compare_id end\n");
	#endif

    if(id1 == 0x55)
    {
        IMM_GetOneChannelValue(0, data, &volt);
	 #ifndef BUILD_LK
	 printk(" lcm_compare_id tcl_oncell lishengli    volt = %d ", volt);
	 #else
	 printf(" lcm_compare_id tcl_onclee lishengli   volt = %d ", volt);
	 #endif

	 if(volt < 100)
	 	return 1;
	 else
		return 0;
    }
    else
        return 0;
#endif
}

// ---------------------------------------------------------------------------
// Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER nt35520_hd720_dsi_cmd_tcl_onc_lcm_drv = 
{
	.name = "nt35520_hd720_tcl_onc",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
#if (LCM_DSI_CMD_MODE)
	.update = lcm_update,
        .set_backlight = lcm_setbacklight,
// .set_pwm = lcm_setpwm,
// .get_pwm = lcm_getpwm,
//.esd_check = lcm_esd_check,
//.esd_recover = lcm_esd_recover,
	.compare_id = lcm_compare_id,
#endif
};
