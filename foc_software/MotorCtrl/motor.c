#include "motor.h"

static ph_abc_t motor_phase_current_from_adcval(ph_abc_t adc_value);
static void motor_phase_current_update_offset(void);
static void motor_update(void);
static void motor_pwm_update(void);
static void motor_apply_pwm_timings(uint16_t timings[3]);
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
    .update = motor_update,
    .pwm_update = motor_pwm_update,
    .apply_pwm_timings = motor_apply_pwm_timings,
};

static ph_abc_t motor_phase_current_from_adcval(ph_abc_t adc_value)
{
    ph_abc_t current = {0};
    if ((adc_value.phA < CURRENT_ADC_LOWER_BOUND || adc_value.phA > CURRENT_ADC_UPPER_BOUND) && (adc_value.phB < CURRENT_ADC_LOWER_BOUND || adc_value.phB > CURRENT_ADC_UPPER_BOUND) && (adc_value.phC < CURRENT_ADC_LOWER_BOUND || adc_value.phC > CURRENT_ADC_UPPER_BOUND))
    {
        return current;
    }

    ph_abc_t adcval_bal = {
        .phA = (float)((int)adc_value.phA - motor.current_offset.phA),
        .phB = (float)((int)adc_value.phB - motor.current_offset.phB),
        .phC = (float)((int)adc_value.phC - motor.current_offset.phC),
    };

    ph_abc_t amp_out_volt = {
        .phA = (3.3f / (float)(1 << 12)) * adcval_bal.phA,
        .phB = (3.3f / (float)(1 << 12)) * adcval_bal.phB,
        .phC = (3.3f / (float)(1 << 12)) * adcval_bal.phC,
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

static void motor_pwm_update(void)
{
    float pwm_timings[3] = {NAN, NAN, NAN};

    svm(motor.foc_.v_alpha, motor.foc_.v_beta, &pwm_timings[0], &pwm_timings[1], &pwm_timings[2]);

    uint16_t next_timings[] = {
        (uint16_t)(pwm_timings[0] * (float)TIM_1_8_PERIOD_CLOCKS),
        (uint16_t)(pwm_timings[1] * (float)TIM_1_8_PERIOD_CLOCKS),
        (uint16_t)(pwm_timings[2] * (float)TIM_1_8_PERIOD_CLOCKS)
    };

    motor.apply_pwm_timings(next_timings);
}

static void motor_update(void)
{
    motor.foc_.update(&motor.foc_);
    
}

static void motor_apply_pwm_timings(uint16_t timings[3])
{
    motor.timer_->Instance->CCR1 = timings[0];
    motor.timer_->Instance->CCR2 = timings[1];
    motor.timer_->Instance->CCR3 = timings[2];
}