#ifndef __ENCODER_BASE_H__
#define __ENCODER_BASE_H__

#include "board.h"


typedef struct encoder_base_t encoder_base_t;
struct encoder_base_t
{
    bool is_ready_;

    int dir;
    int32_t raw;
    int rev;

    float pos;
    float pos_e;
    float pos_s;
    float pos_m;

    float pos_last;
    float pos_dif;

    float spd_mea;


	uint32_t cpr;
	uint8_t bit;
	uint8_t  shift_bit;
	float factor;
	float e_off;
	float m_off;
	float lut[256]; 

    spi_op_t spi_;
    void (*init)(void);
    void (*set_cs)(bool val);
    void (*update_pi_gain)(struct encoder_base_t *self);
    void (*update)(struct encoder_base_t *self);
    void (*sample_now)(struct encoder_base_t *self);
};



#endif