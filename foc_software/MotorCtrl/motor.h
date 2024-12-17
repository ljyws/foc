#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "board.h"
#include "bsp_tim_op.h"
#include "foc.h"

#define MOTOR_TIM_HANDLER htim1

typedef struct
{
    bool pre_calibrated;
    int32_t pole_pairs;

    float calibration_current;
    float phase_current_sample_gain;
    float shunt_conductance_;
    void (*set_phase_inductance)(float value);
    void (*set_phase_resistance)(float value);
    void (*set_current_control_bandwidth)(float value);

}motor_config_t;


typedef struct
{
    motor_config_t config_;
    TIM_HandleTypeDef *timer_;
    foc_t foc_;

    ph_abc_t current_offset;

    bool (*arm)(void);
    bool (*disarm)(bool *was_armed);
    void (*apply_pwm_timings)(uint16_t timings[3], bool tentative);
    void (*update)(void);
    ph_abc_t (*phase_current_from_adcval)(uint32_t *adc_value);
    void (*phase_current_update_offset)(void);

}motor_t;
extern motor_t motor;




#endif
