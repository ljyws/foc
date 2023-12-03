#include "Axis.h"

osThreadId axisTaskHandle;
void axis_thread_start()
{
	osThreadAttr_t axisTask_attributes = {
		.name = "axis_task",
		.stack_size = 256,
		.priority = (osPriority)osPriorityRealtime
	};
	axisTaskHandle = osThreadNew(run_state_mache_loop,NULL,&axisTask_attributes);
}

void run_state_mache_loop(void *argument)
{
	for(;;)
	{
		rgb_set(0,255,255);
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		osDelay(1000);
	}
}