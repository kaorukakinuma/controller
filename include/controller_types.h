/*
 *    file:             controller_types.h
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#ifndef _CONTROLLER_TYPES_H_
#define _CONTROLLER_TYPES_H_

#include <stdint.h>
#include <linux/input.h>

typedef struct Controller Controller;

typedef enum {
    CONTROLLER_E_OK,
    CONTROLLER_E_OBJ,
    CONTROLLER_E_SYS,
} ControllerErcd;

/* Values below needs to be those defined in input.h */
typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t x;
    uint8_t y;
    uint8_t right;
    uint8_t left;
    uint8_t up;
    uint8_t down;
} ControllerConfig;

#endif /* _CONTROLLER_TYPES_H_ */
