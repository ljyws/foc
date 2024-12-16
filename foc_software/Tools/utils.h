#ifndef __UTILS_H__
#define __UTILS_H__

#include "board.h"

#define M_PI (3.14159265358f)
#define M_2PI (6.28318530716f)
#define SQRT3 (1.73205080757f)
#define SQRT3_BY_2 (0.86602540378f)
#define ONE_BY_SQRT3 (0.57735026919f)
#define TWO_BY_SQRT3 (1.15470053838f)

inline int mod(const int dividend, const int divisor)
{
    int r = dividend % divisor;
    if (r < 0)
        r += divisor;
    return r;
}

inline float wrap_pm(float x, float y)
{
#ifdef FPU_FPV4
    float intval = (float)round_int(x / y);
#else
    float intval = nearbyintf(x / y);
#endif
    return x - intval * y;
}

inline float fmodf_pos(float x, float y)
{
    float res = wrap_pm(x, y);
    if (res < 0)
        res += y;
    return res;
}

inline float wrap_pm_pi(float x)
{
    return wrap_pm(x, 2 * M_PI);
}

float sat1_datf(float val, float up, float low);
float fast_atan2(float y, float x);
float fast_sin_f32(float x);
float fast_cos_f32(float x);
#endif
