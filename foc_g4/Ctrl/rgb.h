#ifndef __RGB_H__
#define __RGB_H__


#include "axis.h"
#include "tim.h"

#define ONE_PULSE       	(139)
#define ZERO_PULSE       	(69)
#define RESET_PULSE      	(50)
#define WS_LED_NUMS      	(1)
#define LED_DATA_LEN     	(24)
#define WS2812_DATA_LEN     (WS_LED_NUMS*LED_DATA_LEN)

class Axis;

class RGB
{
public:
    RGB(TIM_HandleTypeDef *timer);

    void setup();

    void setRGB(uint8_t R, uint8_t G, uint8_t B);

    TIM_HandleTypeDef *timer_;

    Axis *axis_ = nullptr;

private:
    uint32_t rgb_buffur[RESET_PULSE + WS2812_DATA_LEN] = { 0 };
};
#endif