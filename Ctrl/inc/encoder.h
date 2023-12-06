#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stdint.h"
#include "stdbool.h"
#include "spi.h"
#include "util.h"
#include "board.h"
#include "math.h"
#define MODE_FLAG_ABS 0x100

typedef enum
{
	ENC_MODE_INCREMENTAL = 0,
	ENC_MODE_SINCOS = 1,
	ENC_MODE_SPI_MT6825 = 256,
}EncoderMode_e;

typedef struct
{
	EncoderMode_e enc_mode_;
	float calib_range; 
	float calib_scan_distance; 
	float calib_scan_omega; 
	float bandwidth;
	int32_t phase_offset;       
	float phase_offset_float; 
	int32_t cpr;   
	int32_t direction;
	float index_offset;
	bool use_index;
	bool pre_calibrated ; 
	bool enable_phase_interpolation;
}EncoderConfig_t;

typedef struct
{
	EncoderConfig_t config_;
	bool is_ready_;
	int32_t shadow_count_;
	int32_t count_in_cpr_;
	float interpolation_;
	float phase_;     
	float phase_vel_;
	float pos_estimate_counts_;  
	float pos_cpr_counts_;  
	float delta_pos_cpr_counts_; 
	float vel_estimate_counts_; 
	float pll_kp_;  
	float pll_ki_;   
	float calib_scan_response_; 
	int32_t pos_abs_;
	float spi_error_rate_;
	float pos_estimate_; 
  float vel_estimate_;
  float pos_circular_; 
	bool pos_estimate_valid_;
  bool vel_estimate_valid_;
}Encoder_t;

extern Encoder_t encoder;

void encoder_init(Encoder_t *encoder_);

void encoder_sample_now(Encoder_t *encoder_);

void encoder_spi_cb(Encoder_t *encoder_);

void encoder_update(Encoder_t *encoder_);

#endif

