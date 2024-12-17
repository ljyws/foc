#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "tim.h"
#include "adc.h"
#include "usbd_cdc_if.h"

#include "utils.h"

#define CURRENT_MEAS_PERIOD ((float)2 * TIM_1_8_PERIOD_CLOCKS * (TIM_1_8_RCR + 1) / (float)TIM_1_8_CLOCK_HZ)
static const float current_meas_period = CURRENT_MEAS_PERIOD;

#define VBUS_S_DIVIDER_RATIO 16.0f
#define CURRENT_SENSE_MIN_VOLT  0.3f
#define CURRENT_SENSE_MAX_VOLT  3.0f

#define SHUNT_RESISTANCE 0.005f

typedef struct 
{
    float phA;
    float phB;
    float phC;
}ph_abc_t;


bool fetch_and_reset_adcs(ph_abc_t *current);
	
#endif
