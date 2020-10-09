/*
 *    file:             controller_types.h
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#ifndef _CONTROLLER_TYPES_H_
#define _CONTROLLER_TYPES_H_

typedef struct Controller Controller;

typedef enum {
    CONTROLLER_E_OK,
    CONTROLLER_E_OBJ,
    CONTROLLER_E_SYS,
} ControllerErcd;

#endif /* _CONTROLLER_TYPES_H_ */
