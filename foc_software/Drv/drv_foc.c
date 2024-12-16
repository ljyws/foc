#include "drv_foc.h"

static void foc_sin_cos_cal(struct foc_t *self);
static void foc_clarke_transform(struct foc_t *self);
static void foc_inverse_clarke(struct foc_t *self);
static void foc_park_transform(struct foc_t *self);
static void foc_inverse_park(struct foc_t *self);
static void foc_update(void);

foc_t foc = {
    .sin_cos_cal = foc_sin_cos_cal,
    .clarke_transform = foc_clarke_transform,
    .inverse_clarke = foc_inverse_clarke,
    .park_transform = foc_park_transform,
    .inverse_park = foc_inverse_park,
};


static void foc_update(void)
{
    foc.theta = mt6825.phase_;
    foc.sin_cos_cal(&foc);
    foc.clarke_transform(&foc);
    foc.park_transform(&foc);
    foc.inverse_park(&foc);
    svm((foc.v_alpha), foc.v_beta, &foc.dtc.phA, &foc.dtc.phB, &foc.dtc.phC);
}


static void foc_sin_cos_cal(struct foc_t *self)
{
    self->sin_val = fast_sin_f32(self->theta);
    self->cos_val = fast_cos_f32(self->theta);
}

static void foc_clarke_transform(struct foc_t *self)
{
    self->i_alpha = self->current_.phA;
    self->i_beta = (self->current_.phB - self->current_.phC) * ONE_BY_SQRT3;
}

static void foc_inverse_clarke(struct foc_t *self)
{
    self->voltage_.phA = self->v_alpha;
    self->voltage_.phB = (-0.5f) * self->v_alpha + SQRT3_BY_2 * self->v_beta;
    self->voltage_.phC = (-0.5f) * self->v_alpha - SQRT3_BY_2 * self->v_beta;
}

static void foc_park_transform(struct foc_t *self)
{
    self->i_d = self->i_alpha * self->cos_val + self->i_beta * self->sin_val;
    self->i_q = self->i_beta * self->cos_val - self->i_alpha * self->sin_val;
}

static void foc_inverse_park(struct foc_t *self)
{
    self->v_alpha = self->v_d * self->cos_val - self->v_q * self->sin_val;
    self->v_beta = self->v_d * self->sin_val + self->v_q * self->cos_val;
}
