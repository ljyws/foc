#include "drv_encoder.h"

static void mt6825_set_spi_cs(bool val);
static void mt6825_sample_now(void);
static void mt6825_abs_spi_start_trans(void);
static bool mt6825_update(void);
static bool mt6825_run_offset_calibration(void);

encoder_t mt6825 = {
    .config_ = {
        .cpr = 262144,
        .direction = 1,
        .bandwidth = 1000,
        .enable_phase_interpolation = true,
        .phase_offset = 0,
        .phase_offset_float = 0.0f,

    },
    .spi_ = {
        .hspi_ = &ENCODER_SPI_HANDLER,
        .cs = {
            .gpio_x = ENCODER_SPI_CS_GPIO_Port,
            .pin = ENCODER_SPI_CS_Pin,
        },
    },
    .count_in_cpr_ = 0,
    .shadow_count_ = 0,
    .pos_cpr_counts_ = 0.0f,
    .pos_estimate_counts_ = 0.0f,
    .vel_estimate_counts_ = 0.0f,
    .delta_pos_cpr_counts_ = 0.0f,
    .pll_ki_ = 0.0f,
    .pll_kp_ = 0.0f,
    .pos_circular_ = 0.0f,
    .interpolation_ = 0.0f,

    .is_ready_ = true,
    .phase_ = 0.0f,
    .phase_vel_ = 0.0f,


    .set_spi_cs = mt6825_set_spi_cs,
    .sample_now = mt6825_sample_now,
    .abs_spi_start_trans = mt6825_abs_spi_start_trans,
    .run_offset_calibration = mt6825_run_offset_calibration,
    .update = mt6825_update,
};

static void mt6825_set_spi_cs(bool val)
{
    gpio_op_write(&mt6825.spi_.cs, val);
}

static void mt6825_sample_now(void)
{
    mt6825.abs_spi_start_trans();
}

static void mt6825_abs_spi_start_trans(void)
{
    uint16_t tx_data[2] = {0x83ff, 0xffff};
    uint16_t rx_data[2];

    mt6825.set_spi_cs(false);
    spi_op_transmit_receive(&mt6825.spi_, tx_data[0], &rx_data[0], 200);
    spi_op_transmit_receive(&mt6825.spi_, tx_data[1], &rx_data[1], 200);
    mt6825.set_spi_cs(true);

    uint32_t raw = ((rx_data[0] & 0x00FF) << 10) | ((rx_data[1] & 0xFC00) >> 6) | ((rx_data[1] & 0x00F0) >> 4);

    mt6825.pos_abs_ = raw;
}

static bool mt6825_run_offset_calibration(void)
{
    
}

static bool mt6825_update(void)
{
    int32_t delta_enc = 0;
    int32_t pos_abs_latched = mt6825.pos_abs_;

    delta_enc = pos_abs_latched - mt6825.count_in_cpr_;
    delta_enc = mod(delta_enc, mt6825.config_.cpr);
    if (delta_enc > mt6825.config_.cpr / 2)
        delta_enc -= mt6825.config_.cpr;

    mt6825.shadow_count_ += delta_enc;
    mt6825.count_in_cpr_ += delta_enc;
    mt6825.count_in_cpr_ = mod(mt6825.count_in_cpr_, mt6825.config_.cpr);

    mt6825.count_in_cpr_ = pos_abs_latched;

    float pos_cpr_counts_last = mt6825.pos_cpr_counts_;

    mt6825.pos_estimate_counts_ += current_meas_period * mt6825.vel_estimate_counts_;
    mt6825.pos_cpr_counts_ += current_meas_period * mt6825.vel_estimate_counts_;

    float delta_pos_counts = (float)(mt6825.shadow_count_ - floor(mt6825.pos_estimate_counts_));
    float delta_pos_cpr_counts = (float)(mt6825.count_in_cpr_ - floor(mt6825.pos_cpr_counts_));

    delta_pos_cpr_counts = wrap_pm(delta_pos_cpr_counts, (float)(mt6825.config_.cpr));
    mt6825.delta_pos_cpr_counts_ += 0.1f * (delta_pos_cpr_counts - mt6825.delta_pos_cpr_counts_);

    mt6825.pos_estimate_counts_ += current_meas_period * mt6825.pll_kp_ * delta_pos_cpr_counts;
    mt6825.pos_cpr_counts_ += current_meas_period * mt6825.pll_kp_ * delta_pos_cpr_counts;
    mt6825.pos_cpr_counts_ = fmodf_pos(mt6825.pos_cpr_counts_, (float)(mt6825.config_.cpr));
    mt6825.vel_estimate_counts_ += current_meas_period * mt6825.pll_ki_ * delta_pos_cpr_counts;
    bool snap_to_zero_vel = false;

    if (fabs(mt6825.vel_estimate_counts_) < 0.5f * current_meas_period * mt6825.pll_ki_)
    {
        mt6825.vel_estimate_counts_ = 0.0f;
        snap_to_zero_vel = true;
    }

    mt6825.pos_estimate_ = mt6825.pos_estimate_counts_ / (float)mt6825.config_.cpr;
    mt6825.vel_estimate_ = mt6825.vel_estimate_counts_ / (float)mt6825.config_.cpr;

    float pos_circular = mt6825.pos_circular_;
    pos_circular += wrap_pm((mt6825.pos_cpr_counts_ - pos_cpr_counts_last) / (float)mt6825.config_.cpr, 1.0f);
    pos_circular = fmodf_pos(pos_circular, 1.0f);
    mt6825.pos_circular_ = pos_circular;

    int32_t corrected_enc = mt6825.count_in_cpr_ - mt6825.config_.phase_offset;
    if (snap_to_zero_vel || !mt6825.config_.enable_phase_interpolation)
        mt6825.interpolation_ = 0.5f;
    else if (delta_enc > 0)
        mt6825.interpolation_ = 0.0f;
    else if (delta_enc < 0)
        mt6825.interpolation_ = 1.0f;
    else
    {
        mt6825.interpolation_ += current_meas_period * mt6825.vel_estimate_counts_;
        if (mt6825.interpolation_ > 1.0f)
            mt6825.interpolation_ = 1.0f;
        if (mt6825.interpolation_ < 0.0f)
            mt6825.interpolation_ = 0.0f;
    }

    float interpolated_enc = corrected_enc + mt6825.interpolation_;

    float elec_rad_per_enc = 14 * 2 * M_PI * (1.0f / (float)(mt6825.config_.cpr));
    float ph = elec_rad_per_enc * (interpolated_enc - mt6825.config_.phase_offset_float);

    if (mt6825.is_ready_)
    {
        mt6825.phase_ = wrap_pm_pi(ph) * mt6825.config_.direction;
        mt6825.phase_vel_ = (2*M_PI) * mt6825.vel_estimate_ * 7 * mt6825.config_.direction;
    }

    return true;
}