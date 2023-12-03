#include "rgb.h"
#include "tim.h"

/******** Glabal Variables ********/

uint32_t rgb_buffur[RESET_PULSE + WS2812_DATA_LEN] = { 0 };

void rgb_set(uint8_t R, uint8_t G, uint8_t B)
{
  uint32_t *p = (rgb_buffur + RESET_PULSE) + (LED_DATA_LEN);
  for (uint16_t i = 0; i < 8; i++)
	{
		p[i] 				= (G << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
		p[i + 8] 		= (R << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
		p[i + 16] 	= (B << i) & (0x80) ? ONE_PULSE : ZERO_PULSE;
	}
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1,rgb_buffur, (82));
}

