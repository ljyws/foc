#ifndef __AXIS_H__
#define __AXIS_H__

#include "cmsis_os.h"
#include "gpio.h"
#include "rgb.h"


typedef enum
{
	AXIS_STATE_UNDEFINED             			= 0,
	AXIS_STATE_IDLE                  			= 1,
	AXIS_STATE_STARTUP_SEQUENCE      			= 2,
	AXIS_STATE_FULL_CALIBRATION_SEQUENCE  = 3,
	AXIS_STATE_MOTOR_CALIBRATION     			= 4,
	AXIS_STATE_ENCODER_INDEX_SEARCH  			= 6,
	AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7,
	AXIS_STATE_CLOSED_LOOP_CONTROL   			= 8,
	AXIS_STATE_LOCKIN_SPIN           			= 9,
	AXIS_STATE_ENCODER_DIR_FIND      			= 10,
	AXIS_STATE_HOMING                			= 11,
	AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION = 12,
	AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION = 13,
}AxisState_e;
	
typedef struct
{
  AxisState_e requested_state_;
}Axis_t;

extern Axis_t *axis;

void run_state_mache_loop(void *argument);
void axis_thread_start();
void axis_init();


#endif

