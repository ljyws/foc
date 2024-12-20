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

    float phase_inductance;
    float phase_resistance;
    float current_control_bandwidth;


    float calibration_current;
    float resistance_calib_max_voltage;
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
    

    bool is_calibrated_;

    bool (*arm)(void);
    bool (*disarm)(bool *was_armed);
    void (*apply_pwm_timings)(uint16_t timings[3]);
    void (*update)(void);
    ph_abc_t (*phase_current_from_adcval)(ph_abc_t adc_value);
    void (*phase_current_update_offset)(void);
    bool (*measure_phase_resistance)(float test_current, float max_voltage);
    bool (*measure_phase_inductance)(float test_voltage);
    bool (*run_calibration)(void);
    void (*pwm_update)(void);
    void (*update_current_controller_gains)(void);

}motor_t;
extern motor_t motor;




#endif
