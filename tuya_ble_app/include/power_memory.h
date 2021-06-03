/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: power_memory.h
 * @Description: �ϵ���书��ʵ��,�رյ�Դ�´ο����󣬻ָ����رյ�Դʱ�Ĺ���״̬
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef POWER_MEMORY_H_
#define POWER_MEMORY_H_

#include "tuya_ble_type.h"

void write_massage_status_to_flash(void);
void read_massage_status_to_flash(void);
void erase_massage_flash(void);

#endif
