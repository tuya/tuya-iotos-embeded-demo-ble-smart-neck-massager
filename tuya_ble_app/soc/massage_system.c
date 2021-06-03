/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: massage_system.c
 * @Description: 工作模式功能实现
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#include "massage_system.h"
#include "gpio_8258.h"
#include "tuya_ble_api.h"
#include "temp_check.h"
#include "voice_prompt.h"

#define DP_BUF_LEN	4
#define BOOST_PWM_CLOCK_HZ	968

extern unsigned long sys_time;

uint8_t app_flag = 0;

MASSAGE_STATE_T massage_state = {
		.alarm = NORMAL,
		.on_off = OFF,
		.pattern = relieve,
		.gear = first_gear,
		.heat = off_heat,
		.adc_flag = 0
};

/*********************************************************************
*   Function:  power_off_init
*   Input:     none
*   Output:    none
*   Return:    none
*   Notice:    Pin configuration of each GPIO port after power off
*********************************************************************/
void power_off_init(void)
{
	// P8、P9、P24、P7、P26、P6 Low level
	pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
	pwm_set_cycle_and_duty(PWM5_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
    gpio_write(HEAT_PIN, 0);
    gpio_write(RS2255_IN_B, 0);
    gpio_write(RS2255_IN_A, 0);
    // P17Configured as an analogue input,Both pins of the voice chip (BUSY, DATA) are set to low input
    gpio_write(GPIO_PD4, 0);
    gpio_write(WTN6_BUSY_PIN, 0);
    gpio_write(WTN6_DATA_PIN, 0);
    pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
    return;
}

void boost_init(void)
{
    gpio_set_func(BOOST_PIN, AS_PWM0);
    gpio_set_output_en(BOOST_PIN, 1);
	//PWM0 1ms cycle
	pwm_set_mode(PWM0_ID, PWM_NORMAL_MODE);
	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, BOOST_PWM_CLOCK_HZ); //Set the PWM frequency for BOOST boost to 16M / (968-1) ≈ 16.55KHz
	pwm_set_phase(PWM0_ID, 0);   //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
	pwm_start(PWM0_ID);
}

void app_dp_handle(uint8_t *dp_data)
{
    printf("dp_data:%d  %d  %d  %d\r\n", dp_data[0], dp_data[1], dp_data[2], dp_data[3]);

    switch (dp_data[0]) {
    case 0x66:
    	if(massage_state.on_off == OFF) {
    		break;
    	}

    	if (dp_data[3] == strong_heat) {
    		massage_state.heat = strong_heat;
    		voice_playing(2);
    	} else {
    		massage_state.heat = off_heat;
    		voice_playing(4);
    	}
    	switching_heat(massage_state.heat);
    	break;
    case 0x67:
    	//printf("dp_data[3]:%d \r\n", dp_data[3]);

    	if(massage_state.on_off == OFF) {
    		break;
    	}

    	switch (dp_data[3]) {
    	case first_gear:
    		massage_state.gear = first_gear;
   			switching_gear(massage_state.gear);
   			voice_playing(massage_state.gear+5);
    		break;
    	case second_gear:
    		massage_state.gear = second_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case third_gear:
    		massage_state.gear = third_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case fourth_gear:
    		massage_state.gear = fourth_gear;
   			switching_gear(massage_state.gear);
   			voice_playing(massage_state.gear+5);
    		break;
    	case fifth_gear:
    		massage_state.gear = fifth_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case sixth_gear:
    		massage_state.gear = sixth_gear;
   		    switching_gear(massage_state.gear);
   		    voice_playing(massage_state.gear+5);
    		break;
    	case seventh_gear:
    		massage_state.gear = seventh_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case eighth_gear:
    		massage_state.gear = eighth_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case ninth_gear:
    		massage_state.gear = ninth_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case tenth_gear:
    		massage_state.gear = tenth_gear;
   			switching_gear(massage_state.gear);
   			voice_playing(massage_state.gear+5);
    		break;
    	case eleventh_gear:
    		massage_state.gear = eleventh_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case twelfth_gear:
    		massage_state.gear = twelfth_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case thirteenth_gear:
    		massage_state.gear = thirteenth_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case fourteenth_gear:
    		massage_state.gear = fourteenth_gear;
   			switching_gear(massage_state.gear);
   			voice_playing(massage_state.gear+5);

    		break;
    	case fifteenth_gear:
    		massage_state.gear = fifteenth_gear;
    		switching_gear(massage_state.gear);
    		voice_playing(massage_state.gear+5);
    		break;
    	case max_gear:
    		massage_state.gear = max_gear;
   		switching_gear(massage_state.gear);
   		voice_playing(massage_state.gear+5);
    		break;
    	default:
    		break;
    	}
        break;
    case 0x68:
    	if(massage_state.on_off == OFF) {
    		break;
    	}

    	if (dp_data[3] == relieve) {
    		massage_state.pattern = relieve;

    	} else if (dp_data[3] == vitality) {
    		massage_state.pattern = vitality;
    	} else if (dp_data[3] == hammering) {
    		massage_state.pattern = hammering;
    	} else if (dp_data[3] == scraping_therapy) {
    		massage_state.pattern = scraping_therapy;
    	} else {
    		massage_state.pattern = intelligent;
    	}
    	voice_playing(massage_state.pattern+21);
    	break;
    case 0x69:
    	if (dp_data[3] == ON) {
    		massage_state.on_off = ON;
    		voice_playing(1);
    		gpio_write(LED_1, 0);
    		rs2255_init();
    		voice_prompt_init();
    		pattern_pin_init();
    		app_flag = 1;
    	} else {
    		massage_state.on_off = OFF;
    		voice_playing(35);
    		gpio_write(LED_1, 1);
    		power_off_init();
    		app_flag = 0;
    	}
    	break;

    default:
    	break;
    }
}
