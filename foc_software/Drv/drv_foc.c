#include "drv_foc.h"

static void foc_sin_cos_cal(struct foc_t *self);
static void foc_clarke_transform(struct foc_t *self);
static void foc_inverse_clarke(struct foc_t *self);
static void foc_park_transform(struct foc_t *self);
static void foc_inverse_park(struct foc_t *self);

foc_t foc = {
    .sin_cos_cal = foc_sin_cos_cal,
    .clarke_transform = foc_clarke_transform,
    .inverse_clarke = foc_inverse_clarke,
    .park_transform = foc_park_transform,
    .inverse_park = foc_inverse_park,
};


static void foc_sin_cos_cal(struct foc_t *self)
{
    self->sin_val = fast_sin_f32(self->theta);
    self->cos_val = fast_cos_f32(self->theta);
}


static void foc_clarke_transform(struct foc_t *self);
static void foc_inverse_clarke(struct foc_t *self);
static void foc_park_transform(struct foc_t *self);
static void foc_inverse_park(struct foc_t *self);