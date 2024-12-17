#include "motor.h"

static float motor_phase_current_from_adcval(uint32_t adc_value);
static float CURRENT_ADC_LOWER_BOUND = (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f);
static float CURRENT_ADC_UPPER_BOUND = (uint32_t)((float)(1 << 12) * CURRENT_SENSE_MIN_VOLT / 3.3f);

motor_t motor = {
    .timer_ = &MOTOR_TIM_HANDLER,
    .config_ = {
        .pre_calibrated = false,
        .phase_current_sample_gain = 20.0f,
        .shunt_conductance_ = (1 / SHUNT_RESISTANCE),
    },
    .foc_ = {
        .sin_cos_cal = foc_sin_cos_cal,
        .clarke_transform = foc_clarke_transform,
        .inverse_clarke = foc_inverse_clarke,
        .park_transform = foc_park_transform,
        .inverse_park = foc_inverse_park,
        .update = foc_update,
    },
    .phase_current_from_adcval = motor_phase_current_from_adcval,

};

static float motor_phase_current_from_adcval(uint32_t adc_value)
{
    if (adc_value < CURRENT_ADC_LOWER_BOUND || adc_value > CURRENT_ADC_UPPER_BOUND)
    {
        return 0;
    }

    int adcval_bal = (int)adc_value - (1 << 11);
    float amp_out_volt = (3.3f / (float)(1 << 12)) * (float)adcval_bal;
    float shunt_volt = amp_out_volt / motor.config_.phase_current_sample_gain;
    float current = shunt_volt * motor.config_.shunt_conductance_;
    return current;
}