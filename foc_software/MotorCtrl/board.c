#include "board.h"
#include "low_level.h"

bool fetch_and_reset_adcs(ph_abc_t *current)
{
//    bool all_adc_done = ((ADC1->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC) && ((ADC2->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC);

//    if (!all_adc_done)
//        return false;

    vbus_sense_adc_cb(ADC2->JDR1);

    uint32_t adc_raw[3];
	adc_raw[0] = ADC1->JDR3;
	adc_raw[1] = ADC1->JDR2;
	adc_raw[2] = ADC1->JDR1;
    *current = motor.phase_current_from_adcval(adc_raw);

    ADC1->ISR = ~(ADC_ISR_JEOC);
    ADC2->ISR = ~(ADC_ISR_JEOC);

    return true;
}