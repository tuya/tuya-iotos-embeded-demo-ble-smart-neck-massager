/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: temp_check.h
 * @Description: ¶ȼ,ADCͨѡ
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#include "temp_check.h"
#include "gpio_8258.h"
#include "pwm.h"
#include "massage_system.h"
#include "tuya_ble_api.h"
#include "voice_prompt.h"

#define R25		10000		//NTC resistance at 25C 10K
#define FULL_POWER_ADC		2000

extern unsigned long sys_time;
unsigned char alarm_buf[]  = {0x65, 0x01, 0x01, 0x00};	// Low battery alarm

void rs2255_init(void)
{
    gpio_set_func(RS2255_IN_A | RS2255_IN_B, AS_GPIO);
    gpio_set_output_en(RS2255_IN_A, 1);
    gpio_set_output_en(RS2255_IN_B, 1);
}

void adc_channel_checkout(rs2255_channel channel)
{
    if(channel >= 4) {
    	TUYA_APP_LOG_ERROR("channel out of range!");
        return;
    }

    switch (channel) {
    case channel_x1:		//Temperature detection
    {
    	gpio_write(RS2255_IN_A, 1);
    	gpio_write(RS2255_IN_B, 0);

    	break;
    }
    case channel_x2:		//Voltage detection
    {
    	TUYA_APP_LOG_DEBUG("enter_X2");
    	gpio_write(RS2255_IN_A, 0);
    	gpio_write(RS2255_IN_B, 1);

    	break;
    }
    case channel_x3:		//boost test
    {
    	gpio_write(RS2255_IN_A, 1);
    	gpio_write(RS2255_IN_B, 1);

    	break;
    }
    default:
        break;
    }
    return;
}

/*Temperature detection, recalled when the heat function is switched on*/
int temperature_detection(void)
{
	int Rntc = 0, Vcc = 0;
	adc_channel_checkout(channel_x1);
	Vcc = adc_sample_and_get_result();	//uint:mv
	Rntc = Vcc*R25 / (3300-Vcc);
	TUYA_APP_LOG_INFO("Rntc_val=%d", Rntc);
	if (Rntc <= 5311) {		//NTC with a resistance of 5311 at 40C
		TUYA_APP_LOG_WARNING("********High Temperature Warning!!!********");
		gpio_write(HEAT_PIN, 0);	//Heating function switched off above 40C
		massage_state.adc_flag = 1;
	}

	return 0;
}

/*Voltage detection*/
int voltage_detection(void)
{
	if (massage_state.adc_flag == 0) {
		return 0;
	}

	if(!clock_time_exceed(sys_time, 300000 * TIME_MS)){	//Battery voltage collection every 5 min
		return 0;
	}
	sys_time = clock_time();

	uint16_t battery_val;
	adc_channel_checkout(channel_x2);
	battery_val = adc_sample_and_get_result();

	/*Low battery alarm on APP side when 10% of battery remaining*/
	if (battery_val < LOW_BATTERY_ALARM) {
		TUYA_APP_LOG_WARNING("Low power!!! Battery level:%dmv", battery_val);
		voice_playing(0x32);	//Insufficient power
		massage_state.alarm = NORMAL;
		tuya_ble_dp_data_report(alarm_buf, 4);
	} else {
		massage_state.alarm = ALARM;
		tuya_ble_dp_data_report(alarm_buf, 4);
	}
	if (battery_val >= FULL_POWER_ADC) {
		voice_playing(0x33);	//Fully charged
	}
	return battery_val;
}

////boostѹ
//int boost_detection(void)
//{
//	uint16_t boost_val;
//	adc_channel_checkout(channel_x3);
//	boost_val = adc_sample_and_get_result();		// uint: mv
//
//	return boost_val;
//}
