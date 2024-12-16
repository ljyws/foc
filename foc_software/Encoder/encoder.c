#include "encoder.h"

static encoder_spi_set_cs(bool val);
static uint32_t read_encoder_spi_raw(void);

encoder_base_t encoder = {
    .is_ready_ = false,
    .dir = 1,
    .bit = 14,
    .cpr = 16384,
    .e_off = 0.0f,
    .m_off = 0.0f,
    .shift_bit = log2f(cpr/256),
    .factor = M_2PI / cpr,
    .spi_ = {
        .hspi_ = &hspi1,
        .cs = {
            .gpio_x = GPIOD,
            .pin = GPIO_PIN_2,
        },
        
    },
    .set_cs = encoder_spi_set_cs,
    .sample_now = read_encoder_spi_raw,

};

static encoder_spi_set_cs(bool val)
{
    gpio_op_write(&encoder.spi_.cs, val);
}

static uint32_t read_encoder_spi_raw(struct encoder_base_t *self)
{
    self->set_cs(false);

}