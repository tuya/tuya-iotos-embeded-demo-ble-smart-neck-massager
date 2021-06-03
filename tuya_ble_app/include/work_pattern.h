/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: work_pattern.h
 * @Description: 工作模式功能实现，共五种工作模式，三种加热模式
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef WORK_PATTERN_H_
#define WORK_PATTERN_H_

#include "pwm.h"
#include "gpio_8258.h"

typedef enum {
	relieve = 0,
	vitality,
	hammering,
	scraping_therapy,
	intelligent
} WORK_PATTERN;

typedef enum {
	strong_heat = 0,
	off_heat = 1
} HEAT_STATE;

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

void pattern_pin_init(void);
void switching_pattern(unsigned char pat);
int switching_heat(unsigned char warm);
void switching_gear(unsigned char gears);

#endif
