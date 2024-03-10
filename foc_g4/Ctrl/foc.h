#ifndef _FOC_H_
#define _FOC_H_

#include "phase_control_law.h"
#include "utils.h"


class FieldOrientedController : public AlphaBetaFrameController
{
public:

    Motor::Error on_measurement(float vbus, alpha_beta_t *Ialpha_beta)final;

    Motor::Error get_alpha_beta_output(alpha_beta_t *mod_alpha_beta, float *ibus)final;

    float p_gain_;
    float i_gain_;

    float theta;
    float sin_val;
    float cos_val;

    Iph_ABC_t iph_i;
    Iph_ABC_t iph_v;

    float I_dq[2];

    float V_dq[2];

    float I_alpha_beta[2];

    float V_alpha_beta[2];

    Iph_ABC_t pwm_out;

    float inv_vbus;

    float *vbus_voltage_measured_;
    alpha_beta_t *Ialpha_beta_measured_;

    float I_measured_report_filter_k_ = 1.0f;

    d_q_t *Idq_setpoint_;
    d_q_t *Vdq_setpoint_;
    float *phase_;
    float *phase_vel_;

    float Id_measured_; // [A]
    float Iq_measured_; // [A]

    float final_v_alpha_ = 0.0f; // [V]
    float final_v_beta_ = 0.0f; // [V]

    float power_ = 0.0f;
};


#endif

