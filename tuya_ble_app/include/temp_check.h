/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: temp_check.h
 * @Description: ¶ȼ
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef TEMP_CHECK_H_
#define TEMP_CHECK_H_

#include "tuya_ble_log.h"
#include "adc.h"




#define LOW_BATTERY_ALARM 		0x640

typedef enum{
    channel_x1 = 0x01,		//ADC1	Temperature detection
    channel_x2,				//ADC2	Voltage detection
    channel_x3				//ADC0	Charge boost test
}rs2255_channel;

void PrintFloat(float value);
void rs2255_init(void);
void adc_channel_checkout(rs2255_channel channel);
int voltage_detection(void);
int temperature_detection(void);
int boost_detection(void);

#endif
