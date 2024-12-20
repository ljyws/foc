#include "low_level.h"

static void adc_sample_enable(void);


float adc_full_scale = 4096.0f;
float adc_ref_voltage = 3.3f;
float vbus_voltage = 12.0f;
float inv_vbus_voltage = 0.0f;

void start_pwm_adc(void)
{
    // init pwm
    int half_load = TIM_1_8_PERIOD_CLOCKS / 2;
    motor.timer_->Instance->CCR1 = half_load;
    motor.timer_->Instance->CCR2 = half_load;
    motor.timer_->Instance->CCR3 = half_load;

    // enable pwm
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2000);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 2000);
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 2000);
    // enable adc
	HAL_ADCEx_Calibration_Start(&ADC_1_HANDLER, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&ADC_2_HANDLER, ADC_SINGLE_ENDED);
    HAL_ADCEx_InjectedStart(&ADC_1_HANDLER);
    HAL_ADCEx_InjectedStart(&ADC_2_HANDLER);
    //delay 2ms
	HAL_Delay(2000);
    //start timer
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 3400);
    __HAL_TIM_ENABLE_IT(motor.timer_, TIM_IT_UPDATE);
	
	motor.phase_current_update_offset();
}

void vbus_sense_adc_cb(uint32_t adc_value)
{
    float voltage_scale = adc_ref_voltage * VBUS_S_DIVIDER_RATIO / adc_full_scale;
    vbus_voltage = adc_value * voltage_scale;
    inv_vbus_voltage = 1 / vbus_voltage;
}

