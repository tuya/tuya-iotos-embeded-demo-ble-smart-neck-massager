/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: massage_system.h
 * @Description: 工作模式功能实现
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef MASSAGE_SYSTEM_H_
#define MASSAGE_SYSTEM_H_

#include "work_pattern.h"
#include <stdbool.h>
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

#define	ALARM	1	//Alarm status
#define NORMAL  0
#define ON		1	//Switch status
#define OFF     0

typedef bool	BOOL;

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
void report_all_dp_data(void);
void boost_init(void);
void app_dp_handle(uint8_t *dp_data);

#endif
