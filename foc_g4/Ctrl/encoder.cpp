#include "encoder.h"

void Encoder::init()
{
    _config.dir = 1;
    _config.raw = 0;
    _config.count = 0;
    _config.count_prev = 0;
    _config.acc_count = 0;
    _config.delta_count = 0;
    _config.offset = 0;
}

uint32_t Encoder::read_raw()
{
    uint8_t data_t[2];
    uint8_t data_r[4];
    uint32_t count;

    data_t[0] = 0x83;
    data_t[1] = 0xff;

    cs_down;
    HAL_SPI_Transmit(&hspi1,&data_t[0],1,1000);
    HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[0],1,1000);
    HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[1],1,1000);
    HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[2],1,1000);

    cs_up;

    data_r[1] >>= 2;
    data_r[2] >>= 4;
    count = data_r[0] << 10 | data_r[1] << 4 | data_r[2] ;

    return count;
}

void Encoder::update()
{
    int raw = read_raw();
    if(_config.dir == +1)
        _config.raw = raw;
    else
        _config.raw = _config.cpr - raw;

    int off_1 = _config.offset_lut[(_config.raw) >> 11];                                        // lookup table lower entry
    int off_2 = _config.offset_lut[((_config.raw >> 11) + 1) % 128];                                // lookup table higher entry
    int off_interp = off_1 + ((off_2 - off_1) * (_config.raw - ((_config.raw >> 11) << 11)) >> 11);     // Interpolate between lookup table entries
    int count = _config.raw - off_interp;

    if (count > _config.cpr)
        count -= _config.cpr;
    if (count < 0)
        count += _config.cpr;
    _config.count = count;

    _config.delta_count = _config.count - _config.count_prev;
    _config.count_prev = _config.count;

    _config.acc_count += _config.delta_count;
}

void Encoder::read_theta(float pp)
{
    int32_t cali_theta;
    update();
    /* ´¦À�«Ʒʽ¾ݻ񵃻úе½ǶȠ*/
    cali_theta = _config.count - _config.offset;
    /*  ¿ؖƼƊýֵԚ±«Ʒµĵ¥֜ƚĚ */
    if (cali_theta > _config.cpr)
        cali_theta -= _config.cpr;
    if (cali_theta < 0)
        cali_theta += _config.cpr;

    mec_angle_360 = cali_theta * (360.0f/ 262144.0f);

    mec_angle =  normalize_angle(cali_theta * (360.0f/ 262144.0f) * (PI / 180.0f));

    elec_angle = normalize_angle(pp * mec_angle);
}

float Encoder::normalize_angle(float angle)
{
    float a = fmod(angle, 2*PI);
    return a >= 0 ? a : (a + 2*PI);
}