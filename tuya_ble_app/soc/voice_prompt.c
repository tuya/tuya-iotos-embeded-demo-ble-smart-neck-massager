/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: voice_prompt.c
 * @Description: 语音播报功能实现(WTN6系列语音芯片， 一线串口通讯)
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#include "voice_prompt.h"
#include "massage_system.h"

void voice_prompt_init(void)
{
    gpio_set_func(WTN6_DATA_PIN | WTN6_BUSY_PIN, AS_GPIO);
    gpio_set_input_en(WTN6_BUSY_PIN, 1);
    gpio_set_output_en(WTN6_DATA_PIN, 1);

    //gpio_write(WTN6_BUSY_PIN, 0);
    gpio_write(WTN6_BUSY_PIN, 1);
    gpio_write(WTN6_DATA_PIN, 1);
}

void voice_playing(uint8_t sb_data)
{
	uint8_t s_data, j;
	bool b_data;
	s_data = sb_data;
	gpio_write(WTN6_DATA_PIN, 0);
	sleep_us(5000);		//delay 5ms
	b_data = s_data & 0X01;

	for (j=0; j<8; j++) {
		if (b_data == 1) {
			gpio_write(WTN6_DATA_PIN, 1);
			sleep_us(600); //delay 600us
			gpio_write(WTN6_DATA_PIN, 0);
			sleep_us(200); //delay 200us
		} else {
			gpio_write(WTN6_DATA_PIN, 1);
			sleep_us(200); //delay 200us
			gpio_write(WTN6_DATA_PIN, 0);
			sleep_us(600); //delay 600us
		}
			s_data = s_data >> 1;
			b_data = s_data & 0X01;
	}
	gpio_write(WTN6_DATA_PIN, 1);
}
