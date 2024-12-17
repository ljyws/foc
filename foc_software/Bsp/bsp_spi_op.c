#include "bsp_spi_op.h"

static inline uint32_t LL_SPI_IsActiveFlag_TXE(SPI_TypeDef *SPIx)
{
    return ((READ_BIT(SPIx->SR, SPI_SR_TXE) == (SPI_SR_TXE)) ? 1UL : 0UL);
}

static inline void LL_SPI_TransmitData16(SPI_TypeDef *SPIx, uint16_t TxData)
{

    SPIx->DR = TxData;
}

static inline uint32_t LL_SPI_IsActiveFlag_BSY(SPI_TypeDef *SPIx)
{
    return ((READ_BIT(SPIx->SR, SPI_SR_BSY) == (SPI_SR_BSY)) ? 1UL : 0UL);
}

static inline uint32_t LL_SPI_IsActiveFlag_RXNE(SPI_TypeDef *SPIx)
{
    return ((READ_BIT(SPIx->SR, SPI_SR_RXNE) == (SPI_SR_RXNE)) ? 1UL : 0UL);
}

static inline uint16_t LL_SPI_ReceiveData16(SPI_TypeDef *SPIx)
{
    return (uint16_t)(READ_REG(SPIx->DR));
}

void spi_op_transmit_receive(struct spi_op_t *self, uint16_t tx_, uint16_t *rx_, uint32_t timeout_ms)
{
    int8_t state = 0;
    *rx_ = 0;
    uint32_t timeout_cnt;
    const uint32_t timeout_cnt_num = timeout_ms;

    /* Check if the SPI is already enabled */
    if ((self->hspi_->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
        /* Enable SPI peripheral */
        __HAL_SPI_ENABLE(self->hspi_);
    }

    /* Wait until TXE flag is set to send data */
    timeout_cnt = 0;
    while (!LL_SPI_IsActiveFlag_TXE(self->hspi_->Instance))
    {
        timeout_cnt++;
        if (timeout_cnt > timeout_cnt_num)
        {
            state = -1;
            break;
        }
    }

    /* Transmit data in 16 Bit mode */
    LL_SPI_TransmitData16(self->hspi_->Instance, tx_);

    /* Check BSY flag */
    timeout_cnt = 0;
    while (LL_SPI_IsActiveFlag_BSY(self->hspi_->Instance))
    {
        timeout_cnt++;
        if (timeout_cnt > timeout_cnt_num)
        {
            state = -1;
            break;
        }
    }

    /* Check RXNE flag */
    timeout_cnt = 0;
    while (!LL_SPI_IsActiveFlag_RXNE(self->hspi_->Instance))
    {
        timeout_cnt++;
        if (timeout_cnt > timeout_cnt_num)
        {
            state = -1;
            break;
        }
    }

    // Read 16-Bits in the data register
    *rx_ = LL_SPI_ReceiveData16(self->hspi_->Instance);

}
