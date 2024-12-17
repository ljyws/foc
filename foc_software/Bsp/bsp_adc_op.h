#ifndef __BSP_ADC_OP_H__
#define __BSP_ADC_OP_H__

#include "board.h"

typedef struct adc_op_t adc_op_t;
struct adc_op_t
{
    ADC_HandleTypeDef *adc_;
    void (*enable_it)(void);
};


void adc_op_enable_it(ADC_HandleTypeDef *adc_);

#endif // !__BSP_ADC_OP_H__
