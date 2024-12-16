#include "bsp_gpio_op.h"

bool gpio_op_read(gpio_op_t *self)
{
    return HAL_GPIO_ReadPin(self->gpio_x, self->pin);
}

void gpio_op_write(gpio_op_t *self, bool val)
{
    HAL_GPIO_WritePin(self->gpio_x, self->pin, val ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void gpio_op_toggle(gpio_op_t *self)
{
    HAL_GPIO_TogglePin(self->gpio_x, self->pin);
}
