#ifndef __UTIL_H__
#define __UTIL_H__

#include "math.h"

#define M_PI 3.14159265358979323846f
#define one_by_sqrt3 0.57735026919f
#define two_by_sqrt3 1.15470053838f
#define sqrt3_by_2 0.86602540378f


inline int mod(const int dividend, const int divisor)
{
  int r = dividend % divisor;
  if (r < 0) r += divisor;
  return r;
}

static inline float fmodf_pos(float x, float y)
{
  float out = fmodf(x, y);
  if (out < 0.0f)
	{
    out += y;
	}
    return out;
}

static inline float wrap_pm(float x, float pm_range)
{
  return fmodf_pos(x + pm_range, 2.0f * pm_range) - pm_range;
}

inline float wrap_pm_pi(float x) 
{
  return wrap_pm(x, 2 * M_PI);
}


#endif
