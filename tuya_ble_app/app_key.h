#ifndef APP_KEY_H_
#define APP_KEY_H_

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

#define key_no           0		// 无作用
#define key_click  	   	 1		// 单击 档位减
#define key_double       2		// 档位加 双击
#define key_long_2s      3
#define key_long_3s      4

void user_button_init(void);
void app_key_poll(void);



//
//extern unsigned char key_read(void);
//void key_init(void);
//static unsigned char key_driver(void);
//unsigned char key_read(void);

//void user_button_init(void);
//void button_scan(void *arg);


#endif
