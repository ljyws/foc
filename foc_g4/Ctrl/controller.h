#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__


#include "axis.h"

class Axis;

class Controller
{
public:

    enum ControlMode {
        CONTROL_MODE_VOLTAGE_CONTROL     = 0,
        CONTROL_MODE_TORQUE_CONTROL      = 1,
        CONTROL_MODE_VELOCITY_CONTROL    = 2,
        CONTROL_MODE_POSITION_CONTROL    = 3,
    };
    enum InputMode {
        INPUT_MODE_INACTIVE              = 0,
        INPUT_MODE_PASSTHROUGH           = 1,
        INPUT_MODE_VEL_RAMP              = 2,
        INPUT_MODE_POS_FILTER            = 3,
        INPUT_MODE_MIX_CHANNELS          = 4,
        INPUT_MODE_TRAP_TRAJ             = 5,
        INPUT_MODE_TORQUE_RAMP           = 6,
        INPUT_MODE_MIRROR                = 7,
        INPUT_MODE_TUNING                = 8,
    };
    struct Config_t
    {
        ControlMode control_mode = CONTROL_MODE_POSITION_CONTROL;
        InputMode input_mode = INPUT_MODE_PASSTHROUGH;
        float pos_gain = 20.0f;                  // [(turn/s) / turn]
        float vel_gain = 1.0f / 6.0f;            // [Nm/(turn/s)]
        Controller *parent;
    };

    bool apply_config();
    void reset();

    Config_t config_;
    Axis *axis_ = nullptr; // set by Axis constructor
};

#endif
