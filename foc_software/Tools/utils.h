#ifndef __UTILS_H__
#define __UTILS_H__

#include "board.h"

#define M_PI (3.14159265358f)
#define M_2PI (6.28318530716f)
#define SQRT3 (1.73205080757f)
#define SQRT3_BY_2 (0.86602540378f)
#define ONE_BY_SQRT3 (0.57735026919f)
#define TWO_BY_SQRT3 (1.15470053838f)



int mod(const int dividend, const int divisor);

float wrap_pm(float x, float y);

float fmodf_pos(float x, float y);

float wrap_pm_pi(float x);

float sat1_datf(float val, float up, float low);
float fast_atan2(float y, float x);
float fast_sin_f32(float x);
float fast_cos_f32(float x);
int svm(float alpha, float beta, float *tA, float *tB, float *tC);
#endif
