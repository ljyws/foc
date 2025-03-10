#include "vofa.h"

#define MAX_BUFFER_SIZE 1024
uint8_t send_buf[MAX_BUFFER_SIZE];
uint16_t cnt = 0;

void vofa_start(void)
{
    vofa_send_data(0,mt6825.phase_);
    vofa_send_data(1, mt6825.pos_abs_);
	vofa_send_data(2, motor.foc_.current_.phA);
	vofa_send_data(3, motor.foc_.current_.phB);
	vofa_send_data(4, motor.foc_.current_.phC);
    vofa_sendframetail();
}

void vofa_transmit(uint8_t *buf, uint16_t len)
{
    //	HAL_UART_Transmit(&huart3, (uint8_t *)buf, len, 0xFFFF);
    CDC_Transmit_FS((uint8_t *)buf, len);
}

void vofa_send_data(uint8_t num, float data)
{
    send_buf[cnt++] = byte0(data);
    send_buf[cnt++] = byte1(data);
    send_buf[cnt++] = byte2(data);
    send_buf[cnt++] = byte3(data);
}

void vofa_sendframetail(void)
{
    send_buf[cnt++] = 0x00;
    send_buf[cnt++] = 0x00;
    send_buf[cnt++] = 0x80;
    send_buf[cnt++] = 0x7f;

    vofa_transmit((uint8_t *)send_buf, cnt);
    cnt = 0;
}

void vofa_demo(void)
{
    static float scnt = 0.0f;

    scnt += 0.01f;

    if (scnt >= 360.0f)
        scnt = 0.0f;

    float v1 = scnt;
    float v2 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
    float v3 = sin((double)(scnt + 120) / 180 * 3.14159) * 180 + 180;
    float v4 = sin((double)(scnt + 240) / 180 * 3.14159) * 180 + 180;

    vofa_send_data(0, v1);
    vofa_send_data(1, v2);
    vofa_send_data(2, v3);
    vofa_send_data(3, v4);

    vofa_sendframetail();
}
