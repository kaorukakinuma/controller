/*
 *    file:             eight_button_controller.h
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#ifndef _EIGHT_BUTTON_CONTROLLER_H_
#define _EIGHT_BUTTON_CONTROLLER_H_

#include "controller_types.h"
Controller * __new__EightButtonController( const char *pPathname, ControllerConfig *pConfig );
Controller * __del__EightButtonController( Controller *pSelf );

#endif /* _EIGHT_BUTTON_CONTROLLER_H_ */
