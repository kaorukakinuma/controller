/*
 *    file:             controller_log.h
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#ifndef _CONTROLLER_LOG_H_
#define _CONTROLLER_LOG_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERRLOG( fmt, ... )\
    (void)printf( "[ \x1b[31mERR\x1b[m ] %s %s() %d, %s: " fmt "\n",\
    __FILE__, __FUNCTION__, __LINE__, strerror(errno), ##__VA_ARGS__ )

#define DBGLOG( fmt, ... )\
    (void)printf( "[ \x1b[36mDBG\x1b[m ] " fmt "\n", ##__VA_ARGS__ )

#endif /* _CONTROLLER_LOG_H_ */
