#include "board.h"
#include "low_level.h"

static bool fetch_and_reset_adcs(ph_abc_t *current)
{
    bool all_adc_done = ((ADC1->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC) && ((ADC2->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC);

    if (!all_adc_done)
        return false;

    vbus_sense_adc_cb(ADC2->JDR1);

    *current->phA = (ADC1->JDR1);
    *current->phB = (ADC1->JDR2);
    *current->phC = (ADC1->JDR3);
}