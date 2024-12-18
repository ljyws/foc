#include "foc.h"



void foc_update(struct foc_t *self)
{
    self->theta = mt6825.phase_;
    self->sin_cos_cal(self);
    self->clarke_transform(self);
    self->park_transform(self);
}


void foc_sin_cos_cal(struct foc_t *self)
{
    self->sin_val = fast_sin_f32(self->theta);
    self->cos_val = fast_cos_f32(self->theta);
}

void foc_clarke_transform(struct foc_t *self)
{
    self->i_alpha = self->current_.phA;
    self->i_beta = (self->current_.phB - self->current_.phC) * ONE_BY_SQRT3;
}

void foc_inverse_clarke(struct foc_t *self)
{
    self->voltage_.phA = self->v_alpha;
    self->voltage_.phB = (-0.5f) * self->v_alpha + SQRT3_BY_2 * self->v_beta;
    self->voltage_.phC = (-0.5f) * self->v_alpha - SQRT3_BY_2 * self->v_beta;
}

void foc_park_transform(struct foc_t *self)
{
    self->i_d = self->i_alpha * self->cos_val + self->i_beta * self->sin_val;
    self->i_q = self->i_beta * self->cos_val - self->i_alpha * self->sin_val;
}

void foc_inverse_park(struct foc_t *self)
{
    self->v_alpha = self->v_d * self->cos_val - self->v_q * self->sin_val;
    self->v_beta = self->v_d * self->sin_val + self->v_q * self->cos_val;
}
