#include <cmath>
#include "foc.h"

Motor::Error AlphaBetaFrameController::on_measurement(float vbus, Iph_ABC_t *currents)
{
    alpha_beta_t Ialpha_beta;
    Ialpha_beta.alpha = currents->phA;
    Ialpha_beta.beta  = one_by_sqrt3 * (currents->phB - currents->phC);

    return on_measurement(vbus, &Ialpha_beta);
}

Motor::Error AlphaBetaFrameController::get_output(Iph_ABC_t *pwm_timings, float *ibus)
{
    alpha_beta_t mod_alpha_beta;
    Motor::Error status = get_alpha_beta_output(&mod_alpha_beta,ibus);
    if(status != Motor::ERROR_NONE)
    {
        return status;
    }
    Iph_ABC_t _ph;
    bool success = SVM(mod_alpha_beta.alpha,mod_alpha_beta.beta,&_ph);
    if(!success)
        return Motor::ERROR_MODULATION_MAGNITUDE;
    pwm_timings->phA = _ph.phA;
    pwm_timings->phB = _ph.phB;
    pwm_timings->phC = _ph.phC;

    return Motor::ERROR_NONE;
}

Motor::Error FieldOrientedController::on_measurement(float vbus, alpha_beta_t *Ialpha_beta)
{
    vbus_voltage_measured_ = &vbus;
    Ialpha_beta_measured_ = Ialpha_beta;
    return Motor::ERROR_NONE;
}

Motor::Error FieldOrientedController::get_alpha_beta_output(alpha_beta_t *mod_alpha_beta, float *ibus)
{
    auto [Vd, Vq] = *Vdq_setpoint_;
    float phase = *phase_;
    float phase_vel = *phase_vel_;
    float vbus_voltage = *vbus_voltage_measured_;

    d_q_t *Idq;

    auto [Ialpha, Ibeta] = *Ialpha_beta_measured_;
    float I_phase;
    float c_I = cosf(I_phase);
    float s_I = sinf(I_phase);

    Idq->d = c_I * Ialpha + s_I * Ibeta;
    Idq->q = c_I * Ibeta - s_I * Ialpha;

    Id_measured_ += I_measured_report_filter_k_ * (Idq->d - Id_measured_);
    Iq_measured_ += I_measured_report_filter_k_ * (Idq->q - Iq_measured_);

    float mod_to_V = (2.0f / 3.0f) * vbus_voltage;
    float V_to_mod = 1.0f / mod_to_V;
    float mod_d;
    float mod_q;

//    if(enable_current_control)

    float pwm_phase;
    float c_p = cosf(pwm_phase);
    float s_p = sinf(pwm_phase);
    float mod_alpha = c_p * mod_d - s_p * mod_q;
    float mod_beta = c_p * mod_q + s_p * mod_d;

    final_v_alpha_ = mod_to_V * mod_alpha;
    final_v_beta_ = mod_to_V * mod_beta;

    *mod_alpha_beta = {mod_alpha, mod_beta};

    auto [Id, Iq] = *Idq;
    *ibus = mod_d * Id + mod_q * Iq;
    power_ = vbus_voltage * (*ibus);

    return Motor::ERROR_NONE;

}
