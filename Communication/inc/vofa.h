#ifndef __VOFA_H__
#define __VOFA_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

void vofa_init();
void vofa_start(void);
void vofa_send_data(uint8_t num, float data);
void vofa_send_frametail(void);

void printTask(void *argument);


#ifdef __cplusplus
}




#endif
#endif


