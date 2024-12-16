#ifndef __DRV_FOC_H__
#define __DRV_FOC_H__

#include "board.h"
#include "drv_encoder.h"

typedef struct foc_t foc_t;
struct foc_t
{
    float vbus;
    float ibus;
    float inv_vbus;

    float duty_now;

    float i_abs;

    float theta;
    float sin_val;
    float cos_val;

    ph_abc_t current_;

    ph_abc_t voltage_;

    float i_d;
    float i_q;

    float v_d;
    float v_q;

    float i_alpha;
    float i_beta;

    float v_alpha;
    float v_beta;

    ph_abc_t dtc;

    void (*sin_cos_cal)(struct foc_t *self);
    void (*clarke_transform)(struct foc_t *self);
    void (*inverse_clarke)(struct foc_t *self);
    void (*park_transform)(struct foc_t *self);
    void (*inverse_park)(struct foc_t *self);
    void (*update)(void);

};

#endif