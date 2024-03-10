#include "motor.h"
#include "foc.h"
#include "phase_control_law.h"


Motor::Motor(TIM_HandleTypeDef* timer
             ):
             timer_(timer){}

struct ResistanceMeasurementControlLaw : AlphaBetaFrameController
{
    void reset() final
    {
        test_voltage_ = 0.0f;
        test_mod_ = -1;
    }

    Motor::Error on_measurement(float vbus, alpha_beta_t *Ialpha_beta)
    {
        if(Ialpha_beta->alpha == 0.0f && Ialpha_beta->beta == 0.0f) {
            actual_current_ = Ialpha_beta->alpha;
//            test_voltage_ += (kI * current_meas_period) * (target_current_ - actual_current_);
//            I_beta_ += (kIBetaFilt * current_meas_period) * (Ialpha_beta->beta - I_beta_);
        }else
        {
            actual_current_ = 0.0f;
            test_voltage_ = 0.0f;
        }
        if(test_voltage_ > max_voltage_)
        {
            test_voltage_ = 0.0f;
            return Motor::ERROR_PHASE_RESISTANCE_OUT_OF_RANGE;
        }else if (vbus == 0.0f)
        {
            return Motor::ERROR_UNKNOWN_VBUS_VOLTAGE;
        }else
        {
            float vfactor = 1.0f / ((2.0f / 3.0f) * vbus);
            test_mod_ = test_voltage_ * vfactor;
            return Motor::ERROR_NONE;
        }
    }

    Motor::Error get_alpha_beta_output(alpha_beta_t *mod_alpha_beta, float *ibus)
    {
        if(test_mod_ == 0.0F)
        {
            return Motor::ERROR_CONTROLLER_INITIALIZING;
        }else
        {
            *mod_alpha_beta = {test_mod_,0.0f};
            *ibus = test_mod_ * actual_current_;
            return Motor::ERROR_NONE;
        }
    };

    float get_resistance()
    {
        return test_voltage_ / target_current_;
    }

    float get_Ibeta()
    {
        return I_beta_;
    }
    const float kI = 1.0f; // [(V/s)/A]
    const float kIBetaFilt = 80.0f;
    float max_voltage_ = 0.0f;
    float actual_current_ = 0.0f;
    float target_current_ = 0.0f;
    float test_voltage_ = 0.0f;
    float I_beta_ = 0.0f; // [A]
    float test_mod_ = -1;
};

struct InductanceMeasurementControlLaw : AlphaBetaFrameController {
    void reset() final
    {
        attached_ = false;
    }

    Motor::Error on_measurement(float vbus, alpha_beta_t *Ialpha_beta)
    {
        if (Ialpha_beta->alpha == 0.0f && Ialpha_beta->beta == 0.0f)
        {
            return Motor::ERROR_UNKNOWN_CURRENT_MEASUREMENT;
        }

        float Ialpha = Ialpha_beta->alpha;

        if (attached_)
        {
            float sign = test_voltage_ >= 0.0f ? 1.0f : -1.0f;
            deltaI_ += -sign * (Ialpha - last_Ialpha_);
        } else
        {
            attached_ = true;
        }
        last_Ialpha_ = Ialpha;
        return Motor::ERROR_NONE;
    }

    Motor::Error get_alpha_beta_output(alpha_beta_t *mod_alpha_beta, float *ibus)
    {
        test_voltage_ *= -1.0f;
        float vfactor = 1.0f / ((2.0f / 3.0f) * vbus_voltage);
        *mod_alpha_beta = {test_voltage_ * vfactor, 0.0f};
        *ibus = 0.0f;
        return Motor::ERROR_NONE;
    }

    float get_inductance()
    {
        float dt = (float)(last_input_timestamp_ - start_timestamp_) / (float)168000000;
        return std::abs(test_voltage_) / (deltaI_ / dt);
    }

    float test_voltage_ = 0.0f;

    bool attached_ = false;
    float sign_ = 0;

    uint32_t start_timestamp_ = 0;
    float last_Ialpha_ = NAN;
    uint32_t last_input_timestamp_ = 0;
    float deltaI_ = 0.0f;
};


bool Motor::arm(PhaseControlLaw *control_law)
{
    control_law_ = control_law;
    axis_->controller_.reset();
    if(control_law_)
        control_law_->reset();
    return true;
}

void Motor::apply_pwm_timings(uint16_t *timings, bool tentative)
{
    TIM_HandleTypeDef *htim = timer_;
    TIM_TypeDef* tim = htim->Instance;

    tim->CCR1 = timings[0];
    tim->CCR2 = timings[1];
    tim->CCR3 = timings[2];

    if (!tentative)
        if (is_armed_)
            tim->BDTR |= TIM_BDTR_AOE;
}

void Motor::update_current_controller_gains()
{
    float p_gain = config_.current_control_bandwidth * config_.phase_inductance;
    float plant_pole = config_.phase_resistance / config_.phase_inductance;
    foc->p_gain_ = p_gain;
    foc->i_gain_ = plant_pole * p_gain;
}

bool Motor::apply_config()
{
    config_.parent = this;
    is_calibrated_ = config_.pre_calibrated;
    update_current_controller_gains();
    return true;
}





