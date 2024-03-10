#ifndef __UTILS_H__
#define __UTILS_H__

#include "foc.h"

constexpr float one_by_sqrt3 = 0.57735026919f;
constexpr float two_by_sqrt3 = 1.15470053838f;
constexpr float sqrt3_by_2 = 0.86602540378f;

bool SVM(float alpha, float beta, Iph_ABC_t *t);

#endif
