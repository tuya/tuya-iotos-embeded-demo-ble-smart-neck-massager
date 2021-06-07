/*
 * @Author: leone
 * @email: liang.zhang@tuya.com
 * @LastEditors: leone
 * @file name: tuya_app_key.h
 * @Description: five-way IO key header file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-06-04
 * @LastEditTime: 2021-06-04
 *
 */
#ifndef _TUYA_APP_KEY_H_
#define _TUYA_APP_KEY_H_

#include "tl_common.h"
#include "blt_soft_timer.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */



#define KEY_RELEAS_LEVEL                   (1)      //1:high level  0:low level


#if KEY_RELEAS_LEVEL
#define KEY_CODE_RELEASE                   0x1F
#define KEY_CODE_SWITCH                    0x0F
#define KEY_HEAT_STRONG           		   0x17
#define KEY_MODE_LOW                       0x1B
#define KEY_MODE_ADD                       0x1D
#define KEY_HEAT_LOW_OFF                   0x1E
#define KEY_LONG_PRESS                     0xFF
#else
#define KEY_CODE_RELEASE                   0x00
#define KEY_CODE_SWITCH                    0x10
#define KEY_HEAT_STRONG           		   0x08
#define KEY_MODE_LOW                       0x04
#define KEY_MODE_ADD                       0x02
#define KEY_HEAT_LOW_OFF                   0x01
#endif


void app_key_init(void);
void app_key_poll(void);

#endif
