#include "axis.h"

static void axis_thread_entry(void *argument);

osThreadId_t AxisTaskHandle;
const osThreadAttr_t axisTask_attributes = {
    .name = "axisTask_attributes",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 256 * 4};

void start_thread(void)
{
    AxisTaskHandle = osThreadNew(axis_thread_entry, NULL, &axisTask_attributes);
}

axis_t axis = {
    .config_ = {
        .startup_motor_calibration = false,
        .startup_encoder_index_search = false,

        .startup_encoder_offset_calibration = false,
        .startup_closed_loop_control = false,
        .startup_homing = false,
    },
    .requested_state_ = AXIS_STATE_STARTUP_SEQUENCE,
};

static void axis_thread_entry(void *argument)
{
    for (;;)
    {
        if (axis.requested_state_ != AXIS_STATE_UNDEFINED)
        {
            size_t pos = 0;

            // 根据requested_state_的值来填充task_chain_
            if (axis.requested_state_ == AXIS_STATE_STARTUP_SEQUENCE)
            {
                if (axis.config_.startup_motor_calibration)
                    axis.task_chain_[pos++] = AXIS_STATE_MOTOR_CALIBRATION;
                if (axis.config_.startup_encoder_offset_calibration)
                    axis.task_chain_[pos++] = AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
                if (axis.config_.startup_homing)
                    axis.task_chain_[pos++] = AXIS_STATE_HOMING;
                if (axis.config_.startup_closed_loop_control)
                    axis.task_chain_[pos++] = AXIS_STATE_CLOSED_LOOP_CONTROL;
                axis.task_chain_[pos++] = AXIS_STATE_IDLE;
            }
            else if (axis.requested_state_ == AXIS_STATE_FULL_CALIBRATION_SEQUENCE)
            {
                axis.task_chain_[pos++] = AXIS_STATE_MOTOR_CALIBRATION;
                axis.task_chain_[pos++] = AXIS_STATE_ENCODER_OFFSET_CALIBRATION;
                axis.task_chain_[pos++] = AXIS_STATE_IDLE;
            }
            else if (axis.requested_state_ != AXIS_STATE_UNDEFINED)
            {
                axis.task_chain_[pos++] = axis.requested_state_;
                axis.task_chain_[pos++] = AXIS_STATE_IDLE;
            }
            axis.task_chain_[pos] = AXIS_STATE_UNDEFINED;
            axis.requested_state_ = AXIS_STATE_UNDEFINED;
        }
        bool status;
        switch (axis.current_state_)
        {
        case AXIS_STATE_MOTOR_CALIBRATION:
        {
            osDelay(2000);
            status = true;
            break;
        }

        case AXIS_STATE_HOMING:
        {
            osDelay(2000);
            status = true;
            break;
        }

        case AXIS_STATE_ENCODER_OFFSET_CALIBRATION:
        {
            osDelay(2000);
            status = true;
            break;
        }

        case AXIS_STATE_LOCKIN_SPIN:
        {
            osDelay(2000);
            status = true;
            break;
        }

        case AXIS_STATE_CLOSED_LOOP_CONTROL:
        {
            osDelay(2000);
            status = true;
            break;
        }

        case AXIS_STATE_IDLE:
        {
            osDelay(2000);
            status = true;
            break;
        }

        default:
        {
            status = false;
            break;
        }
        }

        if (!status)
        {
            memset(axis.task_chain_, AXIS_STATE_UNDEFINED, sizeof(axis.task_chain_));
            axis.current_state_ = AXIS_STATE_IDLE;
        }
        else
        {
            memmove(axis.task_chain_, axis.task_chain_ + 1, sizeof(axis.task_chain_) - sizeof(axis.task_chain_[0]));
            axis.task_chain_[sizeof(axis.task_chain_) - 1] = AXIS_STATE_UNDEFINED;
            axis.current_state_ = axis.task_chain_[0];
        }
    }
}