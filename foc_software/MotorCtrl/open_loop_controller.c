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

// void FOC_voltage(float Vd_set, float Vq_set, float phase)
// {
//     // Clarke transform
//     float i_alpha, i_beta;

//     motor.foc_.clarke_transform()
//     clarke_transform(Foc.i_a, Foc.i_b, Foc.i_c, &i_alpha, &i_beta);

//     // Park transform
//     float i_d, i_q;
//     park_transform(i_alpha, i_beta, phase, &i_d, &i_q);

//     // Used for report
//     Foc.i_q = i_q;
//     UTILS_LP_FAST(Foc.i_q_filt, Foc.i_q, 0.01f);
//     Foc.i_d = i_d;
//     UTILS_LP_FAST(Foc.i_d_filt, Foc.i_d, 0.01f);

//     // Modulation
//     float V_to_mod = 1.0f / (Foc.v_bus_filt * 2.0f / 3.0f);
//     float mod_d    = V_to_mod * Vd_set;
//     float mod_q    = V_to_mod * Vq_set;

//     // Vector modulation saturation, lock integrator if saturated
//     float mod_scalefactor = 0.9f * SQRT3_BY_2 / sqrtf(SQ(mod_d) + SQ(mod_q));
//     if (mod_scalefactor < 1.0f) {
//         mod_d *= mod_scalefactor;
//         mod_q *= mod_scalefactor;
//     }

//     // Inverse park transform
//     float mod_alpha;
//     float mod_beta;
//     inverse_park(mod_d, mod_q, phase, &mod_alpha, &mod_beta);

//     // SVM
//     if (0 == svm(mod_alpha, mod_beta, &Foc.dtc_a, &Foc.dtc_b, &Foc.dtc_c)) {
//         set_a_duty((uint16_t) (Foc.dtc_a * (float) HALF_PWM_PERIOD_CYCLES));
//         set_b_duty((uint16_t) (Foc.dtc_b * (float) HALF_PWM_PERIOD_CYCLES));
//         set_c_duty((uint16_t) (Foc.dtc_c * (float) HALF_PWM_PERIOD_CYCLES));
//     }
// }