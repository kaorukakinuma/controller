/*
 *    file:             eight_button_controller.c
 *    creation date:    2020-10-07
 *    last update:      2020-10-07
 *    author:           kaoru kakinuma
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include <keyboard.h>

#include "eight_button_controller.h"
#include "controller_log.h"
#include "controller.h"

typedef struct {
    Controller                     base;
    bool                           running;
    pthread_t                      thread;
    Keyboard                      *pKeyboard;
    EightButtonControllerConfig    config;
} EightButtonController;

/* ------------------------------------------------------------------------- */

static void * MainThread( void *pArg )
{
    EightButtonController *pSelf = (EightButtonController *)pArg;

    while ( pSelf->running ) {

    }

    pthread_exit( NULL );
}

/* ------------------------------------------------------------------------- */

static ControllerErcd Start( Controller *pSuper )
{
    EightButtonController *pSelf = (EightButtonController *)pSuper;
    if ( pSelf == NULL ) {
        DBGLOG( "Invalid instance." );
        return CONTROLLER_E_OBJ;
    }

    if ( pSelf->running == true ) {
        DBGLOG( "Thread already running." );
        return CONTROLLER_E_OBJ;
    }
    pSelf->running = true;

    pthread_t thread;
    int ret = pthread_create( &thread, NULL, MainThread, pSelf );
    if ( ret != 0 ) {
        ERRLOG( "Thread creation failed." );
        pSelf->running = false;
        return CONTROLLER_E_SYS;
    }
    pSelf->thread = thread;

    return CONTROLLER_E_OK;
}

static ControllerErcd Stop( Controller *pSuper )
{
    EightButtonController *pSelf = (EightButtonController *)pSuper;
    if ( pSelf == NULL ) {
        DBGLOG( "Invalid instance." );
        return CONTROLLER_E_OBJ;
    }

    if ( pSelf->running == false ) {
        DBGLOG( "Thread already stopped." );
        return CONTROLLER_E_OBJ;
    }
    pSelf->running = false;

    int ret = pthread_join( pSelf->thread, NULL );
    if ( ret != 0 ) {
        ERRLOG( "Thread join failed." );
        pSelf->running = true;
        return CONTROLLER_E_SYS;
    }

    return CONTROLLER_E_OK;
}

static const Controller sBase = {
    .Start = Start,
    .Stop  = Stop,
};

/* ------------------------------------------------------------------------- */

Controller * __new__EightButtonController(
    const char *pPathname, EightButtonControllerConfig *pConfig )
{
    if ( pPathname == NULL ) {
        DBGLOG( "Invalid pathname." );
        return NULL;
    }

    /* create instance */
    EightButtonController *pSelf =
        (EightButtonController *)malloc( sizeof(EightButtonController) );
    if ( pSelf == NULL ) {
        ERRLOG( "Malloc failed." );
        return NULL;
    }

    Keyboard *pKeyboard = keyboard_Open( pPathname );
    if ( pKeyboard == NULL ) {
        ERRLOG( "Keyboard open failed." );
        free( pSelf );
        return NULL;
    }

    /* initialize instance */
    pSelf->base      = sBase;
    pSelf->running   = false;
    pSelf->pKeyboard = pKeyboard;
    pSelf->config    = *pConfig;

    return (Controller *)pSelf;
}

Controller * __del__EightButtonController( Controller *pSelf )
{
    if ( pSelf == NULL ) {
        return NULL;
    }
    pSelf->Stop( pSelf );

    EightButtonController *pSub = (EightButtonController *)pSelf;
    keyboard_Close( pSub->pKeyboard );
    free( pSub );

    return NULL;
}
