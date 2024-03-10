#include "main_help.h"

uint32_t cnt = 0;
void Main()
{
    axis.encoder_.init();
    for(;;)
    {
        axis.encoder_.read_theta(14);
        Vofa_Start();
        HAL_Delay(10);
    }
}