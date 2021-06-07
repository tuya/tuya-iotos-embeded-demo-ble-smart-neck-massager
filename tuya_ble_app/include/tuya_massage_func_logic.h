/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_massage_func_logic.h
 * @Description: 工作模式功能实现，共五种工作模式，三种加热模式
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef __TUYA_MASSAGE_FUNC_LOGIC_H_
#define __TUYA_MASSAGE_FUNC_LOGIC_H_

#include <stdbool.h>
#include "pwm.h"
#include "gpio_8258.h"
#include "tuya_ble_type.h"

//功能引脚定义
#define LED_1	GPIO_PD7	// Power indicator
#define KEY_1	GPIO_PC4	// Power, mode plus and minus keys
#define KEY_2	GPIO_PA7	// Heated strong keys
#define KEY_3	GPIO_PC0	// Gear reduction key
#define KEY_4	GPIO_PB7	// Gear plus key
#define KEY_5	GPIO_PB1	// Heating reduction, heating key
#define RS2255_IN_A		GPIO_PD2	// p6
#define RS2255_IN_B     GPIO_PB4	// p26
#define BOOST_PIN		GPIO_PC2	// p8
#define	WTN6_DATA_PIN	GPIO_PA1	// RX2
#define WTN6_BUSY_PIN	GPIO_PA0	// TX2
#define HEAT_PIN	   	GPIO_PC3	// PWM1	P7
#define PATTERN_PIN_A  	GPIO_PC1	// PWM0	P9
#define PATTERN_PIN_B  	GPIO_PB5	// PWM5	P24

#define TIME_MS		   	1000


#define R25		10000		//NTC resistance at 25C 10K
#define FULL_POWER_ADC		2000

typedef bool	BOOL;

typedef enum {
	relieve = 0,
	vitality,
	hammering,
	scraping_therapy,
	intelligent
}WORK_PATTERN;

//1 to 15 steps, max. step -> 15
typedef enum {
	first_gear = 0,
	second_gear,
	third_gear,
	fourth_gear,
	fifth_gear,
	sixth_gear,
	seventh_gear,
	eighth_gear,
	ninth_gear,
	tenth_gear,
	eleventh_gear,
	twelfth_gear,
	thirteenth_gear,
	fourteenth_gear,
	fifteenth_gear,
	max_gear
}WORK_GEARS;

typedef enum {
	strong_heat = 0,
	off_heat = 1
}HEAT_STATE;


typedef enum {
	ALARM = 0,
	NORMAL = 1
}BATTERY_STATE;

typedef enum {
	ON = 0,
	OFF = 1
}POWER_STATE;

// Massager Status
typedef struct {
	BOOL          alarm;
	BOOL		  on_off;
    WORK_PATTERN  pattern;
    WORK_GEARS    gear;
    HEAT_STATE    heat;
    BOOL 		  adc_flag;

}MASSAGE_STATE_T;

extern MASSAGE_STATE_T  massage_state;

void power_off_init(void);
void boost_init(void);
void pattern_pin_init(void);
void switching_pattern(unsigned char pat);
int switching_heat(unsigned char warm);
void switching_gear(unsigned char gears);
void app_dp_handle(uint8_t *dp_data);

#endif
