/*
 *    file:             controller.h
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "controller_types.h"
struct Controller {
    ControllerErcd (*Start)( Controller *pSelf );
    ControllerErcd (*Stop )( Controller *pSelf );
};

#endif /* _CONTROLLER_H_ */
