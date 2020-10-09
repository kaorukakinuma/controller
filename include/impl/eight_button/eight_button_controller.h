/*
 *    file:             eight_button_controller.h
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#ifndef _EIGHT_BUTTON_CONTROLLER_H_
#define _EIGHT_BUTTON_CONTROLLER_H_

#include <stdint.h>
#include <linux/input.h>
#include <com.h>
#include "controller_types.h"

typedef struct {
/* Values below needs to be those defined in input.h */
    uint8_t a;
    uint8_t b;
    uint8_t x;
    uint8_t y;
    uint8_t right;
    uint8_t left;
    uint8_t up;
    uint8_t down;
} EightButtonControllerKeyConfig;

typedef struct {
    const char *pKeyboardPathname;
    Com        *pCom;
    EightButtonControllerKeyConfig keyConfig;
} EightButtonControllerConfig;

Controller * __new__EightButtonController( EightButtonControllerConfig *pConfig );
Controller * __del__EightButtonController( Controller *pSelf );

#endif /* _EIGHT_BUTTON_CONTROLLER_H_ */
