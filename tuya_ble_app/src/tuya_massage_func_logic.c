/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: tuya_massage_mode_manage.c
 * @Description: application function logic realization
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#include "tuya_massage_func_logic.h"
#include "tuya_ble_api.h"
#include "tuya_ble_type.h"
#include "tuya_ble_log.h"
#include "tuya_temp_power_manage.h"
#include "tuya_voice_prompt.h"
#include "random.h"

#define DP_BUF_LEN	4
#define BOOST_PWM_CLOCK_HZ	968

uint8_t app_flag = 0;

MASSAGE_STATE_T massage_state = {
		.alarm = NORMAL,
		.on_off = OFF,
		.pattern = relieve,
		.gear = first_gear,
		.heat = off_heat,
		.adc_flag = 0
};

extern unsigned long sys_time;

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
	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, BOOST_PWM_CLOCK_HZ); //Set the PWM frequency for BOOST boost to 16M / (968-1) 「 16.55KHz
	pwm_set_phase(PWM0_ID, 0);   //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
	pwm_start(PWM0_ID);
}

void pattern_pin_init(void)
{
    gpio_set_func(PATTERN_PIN_A, AS_PWM1_N);
    gpio_set_func(PATTERN_PIN_B, AS_PWM5);
    gpio_set_func(HEAT_PIN, AS_GPIO);

    gpio_set_output_en(PATTERN_PIN_A, 1);
    gpio_set_output_en(PATTERN_PIN_B, 1);
    gpio_set_output_en(HEAT_PIN, 1);

    gpio_write(PATTERN_PIN_A, 0);
    gpio_write(PATTERN_PIN_B, 0);


	//PWM0 1ms cycle  26.5% duty 	1000Hz
	pwm_set_mode(PWM1_ID, PWM_NORMAL_MODE);
	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);
	pwm_set_phase(PWM1_ID, 0);   //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
	pwm_polo_enable(PWM1_ID, 1);	//to enable the pwm polarity
	pwm_start(PWM1_ID);

	//PWM5 1ms cycle  26.5% duty	1000Hz
	pwm_set_mode(PWM5_ID, PWM_NORMAL_MODE);
	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);
	pwm_set_phase(PWM5_ID, 0);   //no phase at pwm beginning
	pwm_set_cycle_and_duty(PWM5_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (265 * CLOCK_SYS_CLOCK_1US) );

}

/*A choice of five operating modes*/
void switching_pattern(unsigned char pat)
{
	if (pat > 4) {
		TUYA_APP_LOG_ERROR("*********No such model!!!**********");
	}

	switch (pat) {
	case relieve:
		pwm_start(PWM5_ID);
		sleep_us(450);		// Delay time of 480us to prevent simultaneous high burnout of the triode
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (265 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(5 * TIME_MS);
		pwm_stop(PWM5_ID);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(30 * TIME_MS);
		break;

	case vitality:
		pwm_start(PWM5_ID);
		sleep_us(450);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (265 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(5 * TIME_MS);
		pwm_stop(PWM5_ID);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(20 * TIME_MS);
		break;

	case hammering:
		pwm_start(PWM5_ID);
		sleep_us(450);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (265 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(5 * TIME_MS);
		pwm_stop(PWM5_ID);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(40 * TIME_MS);
		break;

	case scraping_therapy:
		pwm_start(PWM5_ID);
		sleep_us(450);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (265 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(5 * TIME_MS);
		pwm_stop(PWM5_ID);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(50 * TIME_MS);
		break;

	case intelligent:
		pwm_start(PWM5_ID);
		sleep_us(450);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (265 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(5 * TIME_MS);
		pwm_stop(PWM5_ID);
		pwm_set_cycle_and_duty(PWM1_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (0 * CLOCK_SYS_CLOCK_1US) );
		sleep_us(((rand() % 4 + 2) * 10) * TIME_MS);	// Random delay time 20/30/40/50ms
		break;
	default:
		break;
	}
	return;
}

int switching_heat(unsigned char warm)
{
	if (warm > 1) {
		TUYA_APP_LOG_ERROR("*********No such model!!!**********");
	}

	massage_state.adc_flag = 0;
	switch (warm) {
	case strong_heat:
		TUYA_APP_LOG_INFO("**********strong_heat************");
		gpio_write(HEAT_PIN, 1);
		temperature_detection();
		break;
	case off_heat:
		TUYA_APP_LOG_INFO("**********off_heat************");
		gpio_write(HEAT_PIN, 0);
		break;

	default:
		break;
	}

	return 0;
}

void switching_gear(unsigned char gears)
{
	if (gears > 15) {
		TUYA_APP_LOG_ERROR("*********There is no such gear!!!**********");
	}

	switch (gears) {
	case first_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (10 * CLOCK_SYS_CLOCK_1US) );

		break;
	case second_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (20 * CLOCK_SYS_CLOCK_1US) );

		break;
	case third_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (40 * CLOCK_SYS_CLOCK_1US) );

		break;
	case fourth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (60 * CLOCK_SYS_CLOCK_1US) );

		break;
	case fifth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (70 * CLOCK_SYS_CLOCK_1US) );

		break;
	case sixth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (90 * CLOCK_SYS_CLOCK_1US) );

		break;
	case seventh_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (120 * CLOCK_SYS_CLOCK_1US) );

		break;
	case eighth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (160 * CLOCK_SYS_CLOCK_1US) );

		break;
	case ninth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (180 * CLOCK_SYS_CLOCK_1US) );

		break;
	case tenth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (220 * CLOCK_SYS_CLOCK_1US) );

		break;
	case eleventh_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (240 * CLOCK_SYS_CLOCK_1US) );

		break;
	case twelfth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (260 * CLOCK_SYS_CLOCK_1US) );

		break;
	case thirteenth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (300 * CLOCK_SYS_CLOCK_1US) );

		break;
	case fourteenth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (340 * CLOCK_SYS_CLOCK_1US) );

		break;
	case fifteenth_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (360 * CLOCK_SYS_CLOCK_1US) );

		break;
	case max_gear:
		pwm_set_cycle_and_duty(PWM0_ID, (u16) (1000 * CLOCK_SYS_CLOCK_1US),  (u16) (360 * CLOCK_SYS_CLOCK_1US) );

		break;
	default:
		break;
	}
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
