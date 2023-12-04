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
			encoder_spi_cb();
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
	