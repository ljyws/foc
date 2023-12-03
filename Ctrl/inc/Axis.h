#ifndef __AXIS_H__
#define __AXIS_H__

#include "cmsis_os.h"
#include "gpio.h"
#include "rgb.h"

typedef struct
{
	
}Axis_t;


void run_state_mache_loop(void *argument);
void axis_thread_start();



#endif

