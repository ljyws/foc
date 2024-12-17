#ifndef __BSP_SPI_OPERATION_H__
#define __BSP_SPI_OPERATION_H__

#include <board.h>
#include "bsp_gpio_op.h"


typedef struct spi_op_t spi_op_t;
struct spi_op_t
{
    SPI_HandleTypeDef *hspi_;
    gpio_op_t cs;
    void (*transmit_receive)(void);
};

void spi_op_transmit_receive(struct spi_op_t *self, uint16_t tx_, uint16_t *rx_, uint32_t timeout_ms);
void spi_op_transmit_receive_data16(struct spi_op_t *self);


#endif