#ifndef _CUST_BAT_H_
#define _CUST_BAT_H_

/* stop charging while in talking mode */
#define STOP_CHARGING_IN_TAKLING
#define TALKING_RECHARGE_VOLTAGE 3800
#define TALKING_SYNC_TIME		   60

/* Battery Temperature Protection */
#define MTK_TEMPERATURE_RECHARGE_SUPPORT
#define MAX_CHARGE_TEMPERATURE  61	//50	//Line modified by mickal.ma
#define MAX_CHARGE_TEMPERATURE_MINUS_X_DEGREE	55
#define MIN_CHARGE_TEMPERATURE  -19
#define MIN_CHARGE_TEMPERATURE_PLUS_X_DEGREE	-15
#define ERR_CHARGE_TEMPERATURE  0xFF

/* Linear Charging Threshold */
#define V_PRE2CC_THRES	 		3400	//mV
#define V_CC2TOPOFF_THRES		4050
#define RECHARGING_VOLTAGE      4110
#define CHARGING_FULL_CURRENT    150	//mA

/* Charging Current Setting */
//#define CONFIG_USB_IF 						   
#define USB_CHARGER_CURRENT_SUSPEND			0		// def CONFIG_USB_IF
#define USB_CHARGER_CURRENT_UNCONFIGURED	CHARGE_CURRENT_70_00_MA	// 70mA
#define USB_CHARGER_CURRENT_CONFIGURED		CHARGE_CURRENT_500_00_MA	// 500mA

#define USB_CHARGER_CURRENT					CHARGE_CURRENT_500_00_MA	//500mA
#define AC_CHARGER_CURRENT					CHARGE_CURRENT_550_00_MA  //wangyanhui 
#define NON_STD_AC_CHARGER_CURRENT			CHARGE_CURRENT_500_00_MA
#define CHARGING_HOST_CHARGER_CURRENT       CHARGE_CURRENT_550_00_MA //wangyanhui 
#define APPLE_0_5A_CHARGER_CURRENT          CHARGE_CURRENT_500_00_MA
#define APPLE_1_0A_CHARGER_CURRENT          CHARGE_CURRENT_650_00_MA
#define APPLE_2_1A_CHARGER_CURRENT          CHARGE_CURRENT_800_00_MA


/* Precise Tunning */
#define BATTERY_AVERAGE_DATA_NUMBER	3	
#define BATTERY_AVERAGE_SIZE 	15

/* charger error check */
#define BAT_LOW_TEMP_PROTECT_ENABLE         // stop charging if temp < MIN_CHARGE_TEMPERATURE 
#define V_CHARGER_ENABLE 0				// 1:ON , 0:OFF	
#define V_CHARGER_MAX 6500				// 6.5 V
#define V_CHARGER_MIN 4400				// 4.4 V

/* Tracking TIME */
#define ONEHUNDRED_PERCENT_TRACKING_TIME	20	// 20 second
#define NPERCENT_TRACKING_TIME	   			20	// 20 second
#define SYNC_TO_REAL_TRACKING_TIME  		60	// 60 second
#define V_0PERCENT_TRACKING							3450 //3450mV

/* Battery Notify */
#define BATTERY_NOTIFY_CASE_0001_VCHARGER
#define BATTERY_NOTIFY_CASE_0002_VBATTEMP
//#define BATTERY_NOTIFY_CASE_0003_ICHARGING
//#define BATTERY_NOTIFY_CASE_0004_VBAT
//#define BATTERY_NOTIFY_CASE_0005_TOTAL_CHARGINGTIME

/* JEITA parameter */
//#define MTK_JEITA_STANDARD_SUPPORT
#define CUST_SOC_JEITA_SYNC_TIME 30
#define JEITA_RECHARGE_VOLTAGE  4110	// for linear charging
#define JEITA_TEMP_ABOVE_POS_60_CV_VOLTAGE		BATTERY_VOLT_04_100000_V
#define JEITA_TEMP_POS_45_TO_POS_60_CV_VOLTAGE	BATTERY_VOLT_04_100000_V
#define JEITA_TEMP_POS_10_TO_POS_45_CV_VOLTAGE	BATTERY_VOLT_04_200000_V
#define JEITA_TEMP_POS_0_TO_POS_10_CV_VOLTAGE	BATTERY_VOLT_04_100000_V
#define JEITA_TEMP_NEG_10_TO_POS_0_CV_VOLTAGE	BATTERY_VOLT_03_900000_V
#define JEITA_TEMP_BELOW_NEG_10_CV_VOLTAGE		BATTERY_VOLT_03_900000_V
//Start added by mickal.ma
#define JEITA_TEMP_POS_45_TO_POS_60_CC_CURRENT	CHARGE_CURRENT_550_00_MA	//battery Capacity = 1820mAh 0.3C = 0.3x1820=546
#define JEITA_TEMP_POS_10_TO_POS_45_CC_CURRENT	CHARGE_CURRENT_750_00_MA    //0.5C 0.5x1820=910
#define JEITA_TEMP_POS_0_TO_POS_10_CC_CURRENT	CHARGE_CURRENT_550_00_MA	//0.2C 0.2x1820=364
//End added by mickal.ma



/* For JEITA Linear Charging only */
#define JEITA_NEG_10_TO_POS_0_FULL_CURRENT  120	//mA 
#define JEITA_TEMP_POS_45_TO_POS_60_RECHARGE_VOLTAGE  4000
#define JEITA_TEMP_POS_10_TO_POS_45_RECHARGE_VOLTAGE  4100
#define JEITA_TEMP_POS_0_TO_POS_10_RECHARGE_VOLTAGE   4000
#define JEITA_TEMP_NEG_10_TO_POS_0_RECHARGE_VOLTAGE   3800
#define JEITA_TEMP_POS_45_TO_POS_60_CC2TOPOFF_THRESHOLD	4050
#define JEITA_TEMP_POS_10_TO_POS_45_CC2TOPOFF_THRESHOLD	4050
#define JEITA_TEMP_POS_0_TO_POS_10_CC2TOPOFF_THRESHOLD	4050
#define JEITA_TEMP_NEG_10_TO_POS_0_CC2TOPOFF_THRESHOLD	3850


/* High battery support */
//#define HIGH_BATTERY_VOLTAGE_SUPPORT

/* Disable Battery check for HQA */
#ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
#define CONFIG_DIS_CHECK_BATTERY
#endif

#ifdef MTK_FAN5405_SUPPORT
#define FAN5405_BUSNUM 1
#endif

#ifdef MTK_BQ24158_SUPPORT
#define BQ24158_BUSNUM 1
#endif

//#define MTK_WIRELESS_CHARGER_SUPPORT 1
#endif /* _CUST_BAT_H_ */ 
