#include "motor.h"

static ph_abc_t motor_phase_current_from_adcval(uint32_t *adc_value);
static void motor_phase_current_update_offset(void);
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
    .current_offset = {0},
    .phase_current_update_offset = motor_phase_current_update_offset,
    .phase_current_from_adcval = motor_phase_current_from_adcval,

};

static ph_abc_t motor_phase_current_from_adcval(uint32_t *adc_value)
{
    ph_abc_t current = {0};
//    if ((adc_value[0] < CURRENT_ADC_LOWER_BOUND || adc_value[0] > CURRENT_ADC_UPPER_BOUND) 
//    && (adc_value[1] < CURRENT_ADC_LOWER_BOUND || adc_value[1] > CURRENT_ADC_UPPER_BOUND) 
//    && (adc_value[2] < CURRENT_ADC_LOWER_BOUND || adc_value[2] > CURRENT_ADC_UPPER_BOUND))
//    {
//        return current;
//    }

    uint32_t adcval_bal[3];

    adcval_bal[0] = (int)adc_value[0] - motor.current_offset.phA;
    adcval_bal[1] = (int)adc_value[1] - motor.current_offset.phB;
    adcval_bal[2] = (int)adc_value[2] - motor.current_offset.phC;

    ph_abc_t amp_out_volt = {
        .phA = (3.3f / (float)(1 << 12)) * (float)adcval_bal[0],
        .phB = (3.3f / (float)(1 << 12)) * (float)adcval_bal[1],
        .phC = (3.3f / (float)(1 << 12)) * (float)adcval_bal[2],
    };

    ph_abc_t shunt_volt = {
        .phA = amp_out_volt.phA / motor.config_.phase_current_sample_gain,
        .phB = amp_out_volt.phB / motor.config_.phase_current_sample_gain,
        .phC = amp_out_volt.phC / motor.config_.phase_current_sample_gain,
    };

    current.phA = shunt_volt.phA * motor.config_.shunt_conductance_;
    current.phB = shunt_volt.phB * motor.config_.shunt_conductance_;
    current.phC = shunt_volt.phC * motor.config_.shunt_conductance_;

    return current;
}

static void motor_phase_current_update_offset(void)
{
    float sum_a, sum_b, sum_c;
    for (int i = 0; i < 1000; i++)
    {
        HAL_Delay(1);
        sum_a += (float)(ADC1->JDR3);
        sum_b += (float)(ADC1->JDR2);
        sum_c += (float)(ADC1->JDR1);
    }

    motor.current_offset.phA = sum_a / 1000.0f;
    motor.current_offset.phB = sum_b / 1000.0f;
    motor.current_offset.phC = sum_c / 1000.0f;
}