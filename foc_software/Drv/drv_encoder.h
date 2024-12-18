#ifndef __DRV_ENCODER_H__
#define __DRV_ENCODER_H__

#include "board.h"
#include "bsp_spi_op.h"
#include "utils.h"

#define ENCODER_SPI_HANDLER hspi1
#define ENCODER_SPI_CS_GPIO_Port GPIOD
#define ENCODER_SPI_CS_Pin GPIO_PIN_2

typedef struct
{
    int direction;
    float bandwidth;
    int32_t phase_offset;
    float phase_offset_float;
    int32_t cpr;
    bool enable_phase_interpolation;

} encoder_config_t;

typedef struct
{
    encoder_config_t config_;
    struct spi_op_t spi_;

    bool is_ready_;

    int32_t pos_abs_;

    float pos_cpr_counts_;
    float delta_pos_cpr_counts_;
    float pos_estimate_counts_;
    float vel_estimate_counts_;
    float interpolation_;

    int32_t count_in_cpr_;
    int32_t shadow_count_;
    float pll_kp_;
    float pll_ki_;
    float pos_estimate_;
    float vel_estimate_;
    float pos_circular_;

    float phase_;
    float phase_vel_;

    void (*sample_now)(void);
    void (*abs_spi_start_trans)(void);
    bool (*update)(void);
    void (*set_spi_cs)(bool val);
    bool (*run_offset_calibration)(void);

} encoder_t;
extern encoder_t mt6825;

#endif