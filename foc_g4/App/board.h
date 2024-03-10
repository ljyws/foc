#ifndef __BOARD_H__
#define __BOARD_H__


/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus
extern "C"{
#endif


#ifdef __cplusplus
}
/*---------------------------- C++ Scope ---------------------------*/

#include "rgb.h"
#include "controller.h"
#include "encoder.h"

extern Axis axis;
extern RGB rgb;
extern Encoder mt6825;
extern Controller controller;

#endif
#endif
