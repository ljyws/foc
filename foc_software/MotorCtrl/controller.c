#include "controller.h"

static bool controller_update(void);

controller_t controller = {
    .update = controller_update,
};

static bool controller_update(void)
{
    switch (controller.config_.input_mode)
    {
    case INPUT_MODE_INACTIVE:
    {
        break;
    }

    case INPUT_MODE_PASSTHROUGH:
    {
        controller.pos_setpoint_ = controller.input_pos_;
        controller.vel_setpoint_ = controller.input_vel_;
        controller.torque_setpoint_ = controller.input_torque_;
        break;
    }

    case INPUT_MODE_VEL_RAMP:
    {
        break;
    }
    case INPUT_MODE_TORQUE_RAMP:
    {
        break;
    }
    case INPUT_MODE_POS_FILTER:
    {
        break;
    }
    case INPUT_MODE_TRAP_TRAJ:
    {
        break;
    }
    case INPUT_MODE_TUNING:
    {
        break;
    }

    default:
        break;
    }
	
	return true;
}