#include "low_level.h"

static void adc_sample_enable(void);
static bool fetch_and_reset_adcs(ph_abc_t *cuttrnt);

float adc_full_scale = 4096.0f;
float adc_ref_voltage = 3.3f;
float vbus_voltage = 12.0f;

void start_pwm_adc(void)
{
    // init pwm
    int half_load = TIM_1_8_PERIOD_CLOCKS / 2;
    motor.timer_->Instance->CCR1 = half_load;
    motor.timer_->Instance->CCR2 = half_load;
    motor.timer_->Instance->CCR3 = half_load;

    // enable pwm
    motor.timer_->Instance->CCER |= (TIM_CCx_ENABLE << TIM_CHANNEL_1);
    motor.timer_->Instance->CCER |= (TIM_CCxN_ENABLE << TIM_CHANNEL_1);
    motor.timer_->Instance->CCER |= (TIM_CCx_ENABLE << TIM_CHANNEL_2);
    motor.timer_->Instance->CCER |= (TIM_CCxN_ENABLE << TIM_CHANNEL_2);
    motor.timer_->Instance->CCER |= (TIM_CCx_ENABLE << TIM_CHANNEL_3);
    motor.timer_->Instance->CCER |= (TIM_CCxN_ENABLE << TIM_CHANNEL_3);
	
    // enable adc
    HAL_ADCEx_InjectedStart(&ADC_1_HANDLER);
    HAL_ADCEx_InjectedStart(&ADC_2_HANDLER);
    //delay 2ms

    //start timer
    __HAL_TIM_ENABLE_IT(motor.timer_, TIM_IT_UPDATE);
}

void vbus_sense_adc_cb(uint32_t adc_value)
{
    float voltage_scale = adc_ref_voltage * VBUS_S_DIVIDER_RATIO / adc_full_scale;
    vbus_voltage = adc_value * voltage_scale;
}

