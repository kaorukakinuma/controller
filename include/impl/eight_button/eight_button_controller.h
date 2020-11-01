/*
 * MIT License
 * 
 * Copyright (c) 2020 Kaoru Kakinuma
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _EIGHT_BUTTON_CONTROLLER_H_
#define _EIGHT_BUTTON_CONTROLLER_H_

#include <stdint.h>
#include <linux/input.h>
#include <com.h>
#include "controller_types.h"

typedef struct __attribute__((packed)) {
    uint8_t a;
    uint8_t b;
    uint8_t x;
    uint8_t y;
    uint8_t right;
    uint8_t left;
    uint8_t up;
    uint8_t down;
} EightButtonControllerKey;

typedef union {
    /*
     * 0: Not pressed
     * 1: Pressed
     * 2: Long pressed
     */
    EightButtonControllerKey field;
    char aBulk[sizeof(EightButtonControllerKey)];
} EightButtonControllerData;

typedef struct {
    const char *pKeyboardPathname;
    Com        *pCom;
    /* Values below needs to be those defined in input.h */
    EightButtonControllerKey keyConfig;
} EightButtonControllerConfig;

Controller * __new__EightButtonController( EightButtonControllerConfig *pConfig );
Controller * __del__EightButtonController( Controller *pSelf );

#endif /* _EIGHT_BUTTON_CONTROLLER_H_ */
