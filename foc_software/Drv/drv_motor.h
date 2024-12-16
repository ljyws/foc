#ifndef __DRV_MOTOR_H__
#define __DRV_MOTOR_H__

#include "board.h"
#include "bsp_tim_op.h"

typedef struct
{
    bool pre_calibrated;
    int32_t pole_pairs;

    float calibration_current;
    
    void (*set_phase_inductance)(float value);
    void (*set_phase_resistance)(float value);
    void (*set_current_control_bandwidth)(float value);

}motor_config_t;


typedef struct
{
    motor_config_t config_;
    tim_op_t timer_;

    bool (*arm)(void);
    bool (*disarm)(bool *was_armed);
    void (*apply_pwm_timings)(uint16_t timings[3], bool tentative);

}motor_t;





#endif
