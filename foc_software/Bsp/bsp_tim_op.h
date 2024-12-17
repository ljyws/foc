#ifndef __BSP_TIM_OPERATION_H__
#define __BSP_TIM_OPERATION_H__

#include "board.h"

typedef struct tim_op_t tim_op_t;
struct tim_op_t
{
    TIM_HandleTypeDef *timer_;
    uint32_t channel_;

    void (*init)(struct tim_op_t *self, uint32_t period,uint32_t prescaler);
    void (*start)(struct tim_op_t *self);
    void (*stop)(struct tim_op_t *self);
    void (*set_duty)(struct tim_op_t *self, uint32_t duty);
};

void tim_op_init(tim_op_t *self, uint32_t period,uint32_t prescaler);
void tim_op_start(tim_op_t *self);
void tim_op_stop(tim_op_t *self);
void tim_op_set_duty(tim_op_t *self, uint32_t duty);




#endif // !__BSP_TIM_OPERATION_H__
