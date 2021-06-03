/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: voice_prompt.h
 * @Description: ������������ʵ��(WTN6ϵ������оƬ�� һ�ߴ���ͨѶ)
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef VOICE_PROMPT_H_
#define VOICE_PROMPT_H_

#include "tuya_ble_common.h"
#include "tuya_ble_log.h"
#include "gpio_8258.h"



void voice_playing( uint8_t sb_data);
void voice_prompt_init(void);

#endif
