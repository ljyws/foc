#ifndef ENCODER_H
#define ENCODER_H

#include "axis.h"
#include "cstdint"
#include "spi.h"
#include "cmath"

class Axis;

#define PI 3.1415926f

#define cs_down HAL_GPIO_WritePin(ENC_SPI_CS_GPIO_Port,ENC_SPI_CS_Pin,GPIO_PIN_RESET);
#define cs_up HAL_GPIO_WritePin(ENC_SPI_CS_GPIO_Port,ENC_SPI_CS_Pin,GPIO_PIN_SET);

class Encoder
{
public:

    typedef struct
    {
        int raw = 0;
        int dir = 1;
        uint32_t cpr = 262144;
        int32_t count = 0;
        int32_t count_prev = 0;
        int32_t acc_count = 0;
        int32_t delta_count = 0;
        uint32_t offset = 125289;
        int32_t offset_lut[128];
    }Config_t;

    int32_t offset_lut[128];

    float mec_angle_360;
    float mec_angle;
    float elec_angle;

    uint8_t cnt;
    float speed;
    float angule_speed;
    float last_angle;

    Axis *axis_ = nullptr;

    Config_t _config;

    void init();
    uint32_t read_raw();
    void update();
    void read_theta(float pp);
    float normalize_angle(float angle);
};

#endif
