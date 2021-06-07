/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_dev_data_save.h
 * @Description: 断电记忆功能实现,关闭电源下次开机后，恢复到关闭电源时的工作状态
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#ifndef __TUYA_DEV_DATA_SAVE_H_
#define __TUYA_DEV_DATA_SAVE_H_

#include "tuya_ble_type.h"

void write_massage_status_to_flash(void);
void read_massage_status_to_flash(void);
void erase_massage_flash(void);

#endif
