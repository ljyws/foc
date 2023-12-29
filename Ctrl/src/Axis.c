#include "Axis.h"

osThreadId axisTaskHandle;
void axis_thread_start()
{
	osThreadAttr_t axisTask_attributes = {
		.name = "axis_task",
		.stack_size = 256,
		.priority = (osPriority_t) osPriorityNormal,
	};
	axisTaskHandle = osThreadNew(run_state_mache_loop,NULL,&axisTask_attributes);
}

void run_state_mache_loop(void *argument)
{
	axis_init();
	for(;;)
	{
		if(axis->requested_state_ != AXIS_STATE_UNDEFINED)
		{
			int pos = 0;
		
		}
		osDelay(100);
	}
}


void axis_init()
{
	axis->requested_state_ = AXIS_STATE_STARTUP_SEQUENCE;
}




