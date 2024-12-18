#include "board.h"
#include "low_level.h"
#include "open_loop_controller.h"
bool fetch_and_reset_adcs(ph_abc_t *current)
{
    bool all_adc_done = ((ADC1->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC) && ((ADC2->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC);

    if (!all_adc_done)
        return false;

    vbus_sense_adc_cb(ADC2->JDR1);

    ph_abc_t adc_raw = {
        .phA = (ADC1->JDR3),
        .phB = (ADC1->JDR2),
        .phC = (ADC1->JDR1),
    };

    *current = motor.phase_current_from_adcval(adc_raw);

    ADC1->ISR = ~(ADC_ISR_JEOC);
    ADC2->ISR = ~(ADC_ISR_JEOC);

    return true;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);

        mt6825.sample_now();

        ph_abc_t current_;
        fetch_and_reset_adcs(&current_);
		mt6825.update();
        open_loop_controller.update(0.2, 0);
        // update
        //  mt6825.update();
        //  motor.update();

        // pwm_update
        //  motor.pwm_update();
    }
}
