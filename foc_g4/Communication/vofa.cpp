#include "vofa.h"

/*
************************************************************************
* @brief      发送数据给上位机
* @param      NULL
* @return     void
* describe:   NULL
************************************************************************
*/
extern uint32_t cnt;
void Vofa_Start(void)
{
//	Vofa_SendData(0,(float)0x80);
    Vofa_SendData(1, axis.encoder_.mec_angle);
    Vofa_Sendframetail();
}
/*
************************************************************************
* @brief      发送数据给上位机
* @param      NULL
* @return     void
* describe:   NULL
************************************************************************
*/
void Vofa_SendData(uint8_t num, float data)
{
    // 发送数据
    float Data[1];
    Data[0] = data;
    CDC_Transmit_FS((uint8_t*)Data,sizeof(Data));
//    HAL_UART_Transmit(&huart1,(uint8_t*)Data,sizeof(Data),0XFFFF);
}
/*
************************************************************************
* @brief      给每个数据包发送帧尾
* @param      NULL
* @return     void
* describe:   NULL
************************************************************************
*/
void Vofa_Sendframetail(void)
{
    // 发送帧尾
    char tail[4] = {0x00, 0x00, 0x80, 0x7f};
//    HAL_UART_Transmit(&huart1,(uint8_t*)tail,sizeof(tail),0XFFFF);
    CDC_Transmit_FS((uint8_t*)tail,sizeof(tail));
}

