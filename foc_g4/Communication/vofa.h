#ifndef __VOFA_H
#define __VOFA_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "usbd_cdc_if.h"
#include "board.h"

void Vofa_Start(void);
void Vofa_SendData(uint8_t num, float data);
void Vofa_Sendframetail(void);


#ifdef __cplusplus
}




#endif
#endif

