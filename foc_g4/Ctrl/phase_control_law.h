#ifndef __PHASE_CONTROL_LAW_H__
#define __PHASE_CONTROL_LAW_H__

#include "motor.h"

class Motor;

class PhaseControlLaw
{
public:
    virtual void reset() = 0;

    virtual Motor::Error on_measurement(float vbus, Iph_ABC_t *currents) = 0;

    virtual Motor::Error get_output(Iph_ABC_t *pwm_timings, float *ibus) = 0;
};

class AlphaBetaFrameController : public PhaseControlLaw
{
private:
    Motor::Error on_measurement(float vbus, Iph_ABC_t *currents)final;

    Motor::Error get_output(Iph_ABC_t *pwm_timings, float *ibus)final;

protected:
    virtual Motor::Error on_measurement(float vbus, alpha_beta_t *Ialpha_beta) = 0;

    virtual Motor::Error get_alpha_beta_output(alpha_beta_t *mod_alpha_beta, float *ibus) = 0;
};

#endif
