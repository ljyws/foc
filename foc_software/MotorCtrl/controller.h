#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "board.h"

typedef enum
{
    CONTROL_MODE_VOLTAGE_CONTROL = 0,
    CONTROL_MODE_TORQUE_CONTROL = 1,
    CONTROL_MODE_VELOCITY_CONTROL = 2,
    CONTROL_MODE_POSITION_CONTROL = 3,
} controller_mode_e;

typedef enum
{
    INPUT_MODE_INACTIVE                      = 0,
    INPUT_MODE_PASSTHROUGH                   = 1,
    INPUT_MODE_VEL_RAMP                      = 2,
    INPUT_MODE_POS_FILTER                    = 3,
    INPUT_MODE_MIX_CHANNELS                  = 4,
    INPUT_MODE_TRAP_TRAJ                     = 5,
    INPUT_MODE_TORQUE_RAMP                   = 6,
    INPUT_MODE_TUNING                        = 7,
} controller_input_e;

typedef struct
{
    controller_mode_e control_mode_;
    controller_input_e input_mode;

    float pos_gain;
    float vel_gain;
    float vel_integrator_gain;
    float vel_limit;
    float vel_tolerance;
    float vel_ramp_rate;
    float torque_ramp_rate;
    void (*set_conntrol_mode)(controller_mode_e val);
}controller_config_t;

typedef struct
{
    controller_config_t config_;
    float voltage_setpoint_;
    float pos_setpoint_;
    float vel_setpoint_;
    float torque_setpoint_;
    float vel_integrator_torque_;

    float input_voltage_;
    float input_pos_; 
    float input_vel_;    
    float input_torque_; 
    float input_filter_kp_;
    float input_filter_ki_;

    bool (*update)(void);
} controller_t;

#endif // !__CONTROLLER_H__
