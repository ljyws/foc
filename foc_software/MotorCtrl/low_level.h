#ifndef __LOW_LEVEL_H__
#define __LOW_LEVEL_H__

#include "board.h"
#include "motor.h"
#include "bsp_adc_op.h"


#define ADC_1_HANDLER hadc1
#define ADC_2_HANDLER hadc2

extern float vbus_voltage;
extern float inv_vbus_voltage;


void vbus_sense_adc_cb(uint32_t adc_value);
void start_pwm_adc(void);


#endif // !__LOW_LEVEL_H__