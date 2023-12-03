#include "usbd_cdc_if.h"
#include "vofa.h"
#include "cmsis_os.h"
/*
************************************************************************
* @brief      发送数据给上位机
* @param      NULL
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_start(void)
{
		vofa_send_data(0,(float)0x80);
    vofa_send_frametail();
}
/*
************************************************************************
* @brief      发送数据给上位机
* @param      NULL
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_send_data(uint8_t num, float data)
{
  // 发送数据
  float Data[1];
  Data[0] = data;
  CDC_Transmit_FS((uint8_t*)Data,sizeof(Data));
	//HAL_UART_Transmit(&huart1,(uint8_t*)Data,sizeof(Data),0XFFFF);
}
/*
************************************************************************
* @brief      给每个数据包发送帧尾
* @param      NULL
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_send_frametail(void)
{
  // 发送帧尾
  char tail[4] = {0x00, 0x00, 0x80, 0x7f};
	CDC_Transmit_FS((uint8_t*)tail,sizeof(tail));
	//HAL_UART_Transmit(&huart1,(uint8_t*)tail,sizeof(tail),0XFFFF);
}


osThreadId printTaskHandle;
void vofa_init()
{
	const osThreadAttr_t printTask_attributes = {
				.name = "printTask",
				.stack_size = 128,
				.priority = (osPriority_t) osPriorityNormal,
	};
  printTaskHandle = osThreadNew(printTask, NULL,&printTask_attributes);
}


void printTask(void *argument)
{
  for(;;)
  {
    vofa_start();
    osDelay(1);
  }
}














