#include "open_loop_controller.h"

static void open_loop_controller_update(float vd_ref, float vq_ref);

open_loop_controller_t open_loop_controller = {
    .pos_acc_ = 0.0025f,
    .pos_ = 0,

    .foc_ = {
        .sin_cos_cal = foc_sin_cos_cal,
        .clarke_transform = foc_clarke_transform,
        .inverse_clarke = foc_inverse_clarke,
        .park_transform = foc_park_transform,
        .inverse_park = foc_inverse_park,
        .update = foc_update,
    },

    .update = open_loop_controller_update,
};

void open_loop_controller_update(float vd_ref, float vq_ref)
{
    open_loop_controller.pos_ += open_loop_controller.pos_acc_;
    wrap_0_2pi(open_loop_controller.pos_);
    open_loop_controller.foc_.theta = open_loop_controller.pos_;
    open_loop_controller.foc_.sin_cos_cal(&open_loop_controller.foc_);
    open_loop_controller.foc_.clarke_transform(&open_loop_controller.foc_);
    open_loop_controller.foc_.park_transform(&open_loop_controller.foc_);
    open_loop_controller.foc_.v_d = vd_ref;
    open_loop_controller.foc_.v_q = vq_ref;
    open_loop_controller.foc_.inverse_park(&open_loop_controller.foc_);

    float pwm_timings[3] = {NAN, NAN, NAN};

    svm(open_loop_controller.foc_.v_alpha * (inv_vbus_voltage), 
        open_loop_controller.foc_.v_beta * (inv_vbus_voltage), 
        &pwm_timings[0], 
        &pwm_timings[1], 
        &pwm_timings[2]
    );

    uint16_t next_timings[] = {
        (uint16_t)(pwm_timings[0] * (float)TIM_1_8_PERIOD_CLOCKS),
        (uint16_t)(pwm_timings[1] * (float)TIM_1_8_PERIOD_CLOCKS),
        (uint16_t)(pwm_timings[2] * (float)TIM_1_8_PERIOD_CLOCKS)};

    motor.timer_->Instance->CCR1 = next_timings[0];
    motor.timer_->Instance->CCR2 = next_timings[1];
    motor.timer_->Instance->CCR3 = next_timings[2];
}