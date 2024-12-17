#include "bsp_adc_op.h"

void adc_op_enable_it(ADC_HandleTypeDef *adc_)
{
    HAL_ADCEx_InjectedStart(adc_);
}

