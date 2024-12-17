#ifndef __BSP_GPIO_OPERATION_H__
#define __BSP_GPIO_OPERATION_H__

#include"board.h"


typedef struct gpio_op_t gpio_op_t;
struct gpio_op_t {
    GPIO_TypeDef *gpio_x;
    uint32_t pin;
    bool (*read)(void);
    void (*write)(bool val);
    void (*toggle)(void);
};

bool gpio_op_read(gpio_op_t *self);
void gpio_op_write(gpio_op_t *self, bool val);
void gpio_op_toggle(gpio_op_t *self);



#endif //!__BSP_GPIO_OPERATION_H__
