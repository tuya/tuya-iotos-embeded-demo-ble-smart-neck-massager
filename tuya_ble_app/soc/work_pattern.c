/*
 * @Author: zzw
 * @email: huanling.zhang@tuya.com
 * @LastEditors: zzw
 * @file name: work_pattern.c
 * @Description: 工作模式功能实现，共五种工作模式，三种加热模式
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-22
 * @LastEditTime: 2021-05-25
 *
 */

#include "work_pattern.h"
#include "tuya_ble_type.h"
#include "massage_system.h"
#include "tuya_ble_log.h"
#include "temp_check.h"
#include "random.h"

extern unsigned long sys_time;

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
//    gpio_write(HEAT_PIN, 1);

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
//	printf("wram%d massage_state.heat%d\r\n", warm, massage_state.heat);
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

