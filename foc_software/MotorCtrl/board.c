#include "board.h"
#include "low_level.h"
#include "open_loop_controller.h"
#include "vofa.h"

#define ControlLoop_IRQHandler USB_HP_IRQHandler
#define ControlLoop_IRQn USB_HP_IRQn

bool board_init()
{
    HAL_NVIC_SetPriority(ControlLoop_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ControlLoop_IRQn);

    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);

    return true;
}

bool fetch_and_reset_adcs(ph_abc_t *current)
{
    //    bool all_adc_done = ((ADC1->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC) && ((ADC2->ISR & ADC_ISR_JEOC) == ADC_ISR_JEOC);

    //    if (!all_adc_done)
    //        return false;

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

volatile bool counting_down_ = false;
void tim1_period_elapsed_callback(void)
{
    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
    vofa_start();
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    bool counting_down = TIM1->CR1 & TIM_CR1_DIR;
    bool timer_update_missed = (counting_down_ == counting_down);
    counting_down_ = counting_down;

    if (!counting_down)
    {
        mt6825.sample_now();
        NVIC->STIR = ControlLoop_IRQn;
    }
    else
    {
        TIM1->CCR1 =
            TIM1->CCR2 =
                TIM1->CCR3 =
                    TIM_1_8_PERIOD_CLOCKS / 2;
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);

    //        ph_abc_t current_;

    // open_loop_controller.update(0.5, 0);
    //  update
    //   mt6825.update();
    //   motor.update();

    // pwm_update
    //  motor.pwm_update();
}

void ControlLoop_IRQHandler(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    fetch_and_reset_adcs(&motor.foc_.current_);
    mt6825.update();
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}