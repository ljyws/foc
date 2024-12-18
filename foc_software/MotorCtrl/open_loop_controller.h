#ifndef __OPEN_LOOP_CONTROLLER_H__
#define __OPEN_LOOP_CONTROLLER_H__

#include "board.h"
#include "low_level.h"
#include "foc.h"

typedef struct
{
    float pos_acc_;
    float pos_;

    foc_t foc_;
    void (*update)(float vd_ref, float vq_ref);
}open_loop_controller_t;
extern open_loop_controller_t open_loop_controller;




#endif // !__OPEN_LOOP_CONTROLLER_H__
