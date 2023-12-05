#ifndef __UTIL_H__
#define __UTIL_H__

#include "math.h"

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




#endif
