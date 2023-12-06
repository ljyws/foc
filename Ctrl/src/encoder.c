#include "encoder.h"

void encoder_sample_now(Encoder_t *encoder_)
{
	switch(encoder_->config_.enc_mode_)
	{
		case ENC_MODE_INCREMENTAL:
		{
			
		}break;
		case ENC_MODE_SINCOS:
		{
			
		}break;
		case ENC_MODE_SPI_MT6825:
		{
			encoder_spi_cb(encoder_);
		}break;
		default:break;
	}	
}

void encoder_spi_cb(Encoder_t *encoder_)
{
	uint32_t pos;
	switch(encoder_->config_.enc_mode_)
	{
		case ENC_MODE_SPI_MT6825:
		{
			uint8_t data_t[2];
			uint8_t data_r[4];
			data_t[0] = 0x83;
			data_t[1] = 0xff;
			HAL_GPIO_WritePin(ENC_SPI_CS_GPIO_Port,ENC_SPI_CS_Pin,GPIO_PIN_RESET);
			HAL_SPI_Transmit(&hspi3,&data_t[0],1,1000);
			HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[0],1,1000);
			HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[1],1,1000);
			HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[2],1,1000);
			HAL_GPIO_WritePin(ENC_SPI_CS_GPIO_Port,ENC_SPI_CS_Pin,GPIO_PIN_SET);

			data_r[1] >>= 2;
			data_r[2] >>= 4;
			pos = data_r[0] << 10 | data_r[1] << 4 | data_r[2] ;
		}break;
		default:break;
	}
	encoder_->pos_abs_ = pos;
	
}
	

void encoder_update(Encoder_t *encoder_)
{
	int32_t delta_enc = 0;
  int32_t pos_abs_latched = encoder_->pos_abs_;
	switch(encoder_->config_.enc_mode_)
	{
		case ENC_MODE_INCREMENTAL:
		{
			
		}break;
		case ENC_MODE_SINCOS:
		{
			
		}break;
		case ENC_MODE_SPI_MT6825:
		{
			delta_enc = pos_abs_latched - encoder_->count_in_cpr_;
			delta_enc = mod(delta_enc, encoder_->config_.cpr);
			if(delta_enc > encoder_->config_.cpr/2)
			{
				delta_enc -= encoder_->config_.cpr;
			}
		}break;
		default:break;
	}
	encoder_->shadow_count_ += delta_enc;
	encoder_->count_in_cpr_ += delta_enc;
	encoder_->count_in_cpr_ = mod(encoder_->count_in_cpr_, encoder_->config_.cpr);
	if(encoder_->config_.enc_mode_ & MODE_FLAG_ABS)
	{
		encoder_->count_in_cpr_ = pos_abs_latched;
	}
	float pos_cpr_counts_last = encoder_->pos_cpr_counts_;
	
	encoder_->pos_estimate_counts_ += current_meas_period * encoder_->vel_estimate_counts_;
	encoder_->pos_cpr_counts_ 		 += current_meas_period * encoder_->vel_estimate_counts_;
	
	float delta_pos_counts = (float)(encoder_->shadow_count_ - floor(encoder_->pos_estimate_counts_));
	float delta_pos_cpr_counts = (float)(encoder_->count_in_cpr_ - floor(encoder_->pos_cpr_counts_));
	
	delta_pos_cpr_counts = wrap_pm(delta_pos_cpr_counts, (float)(encoder_->config_.cpr));
	encoder_->delta_pos_cpr_counts_ += 0.1f * (delta_pos_cpr_counts - encoder_->delta_pos_cpr_counts_);
	
	encoder_->pos_estimate_counts_ += current_meas_period * encoder_->pll_kp_ * delta_pos_counts;
	encoder_->pos_cpr_counts_ += current_meas_period * encoder_->pll_kp_ * delta_pos_cpr_counts;
	encoder_->pos_cpr_counts_ = fmodf_pos(encoder_->pos_cpr_counts_, (float)encoder_->config_.cpr);
	encoder_->vel_estimate_counts_ += current_meas_period * encoder_->pll_ki_ * delta_pos_cpr_counts;
	bool snap_to_zero_vel = false;
	if(fabs(encoder_->vel_estimate_counts_) < 0.5f * current_meas_period * encoder_->pll_ki_)
	{
		encoder_->vel_estimate_counts_ = 0.0f;
		snap_to_zero_vel = true;
	}
	
	encoder_->pos_estimate_ = encoder_->pos_estimate_counts_ / (float)(encoder_->config_.cpr);
	encoder_->vel_estimate_ = encoder_->vel_estimate_counts_ / (float)(encoder_->config_.cpr);
	
	int32_t corrected_enc = encoder_->count_in_cpr_ - encoder_->config_.phase_offset;
	if(snap_to_zero_vel || !encoder_->config_.enable_phase_interpolation)
	{
		encoder_->interpolation_ = 0.5f;
	}else if(delta_enc > 0.0f)
	{
		encoder_->interpolation_ = 0.0f;
	}else if(delta_enc < 0.0f)
	{
		encoder_->interpolation_ = 1.0f;
	}else
	{
		encoder_->interpolation_ += current_meas_period * encoder_->vel_estimate_counts_;
    if (encoder_->interpolation_ > 1.0f) 
			encoder_->interpolation_ = 1.0f;
    if (encoder_->interpolation_ < 0.0f) 
			encoder_->interpolation_ = 0.0f;
	}
	float interpolated_enc = corrected_enc + encoder_->interpolation_;
	
	float enec_rad_per_enc = motor.config_.pole_pairs * 2.0f * M_PI * (1.0f / (float)(encoder_->config_.cpr)); 
	float ph = enec_rad_per_enc * (interpolated_enc - encoder_->config_.phase_offset_float);
	
	if(encoder_->is_ready_)
	{
		encoder_->phase_ = wrap_pm_pi(ph) * encoder_->config_.direction;
		encoder_->phase_vel_ = (2.0f * M_PI) * encoder_->vel_estimate_ * motor.config_.pole_pairs * encoder_->config_.direction;
	}
	
}












