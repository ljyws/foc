#ifndef __AXIS_H__
#define __AXIS_H__

#include "board.h"
#include "drv_led.h"

typedef enum
{
    AXIS_STATE_UNDEFINED = 0,
    AXIS_STATE_IDLE = 1,
    AXIS_STATE_STARTUP_SEQUENCE = 2,
    AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,
    AXIS_STATE_MOTOR_CALIBRATION = 4,
    AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 5,
    AXIS_STATE_CLOSED_LOOP_CONTROL = 6,
    AXIS_STATE_LOCKIN_SPIN = 7,
    AXIS_STATE_ENCODER_DIR_FIND = 8,
    AXIS_STATE_HOMING = 9,
} axis_state_e;

typedef struct
{
    bool startup_motor_calibration;        
    bool startup_encoder_index_search;      
                                                    
    bool startup_encoder_offset_calibration; 
    bool startup_closed_loop_control;      
    bool startup_homing;                   
} axis_config_t;

typedef struct
{
    axis_config_t config_;
    axis_state_e requested_state_;
    axis_state_e current_state_;
    axis_state_e task_chain_[10];
} axis_t;

void start_thread(void);

#endif