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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include <keyboard.h>

#include "eight_button_controller.h"
#include "controller_log.h"
#include "controller.h"

typedef struct {
    Controller                        base;
    bool                              running;
    pthread_t                         thread;
    Keyboard                         *pKeyboard;
    Com                              *pCom;
    EightButtonControllerKeyConfig    keyConfig;
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

Controller * __new__EightButtonController( EightButtonControllerConfig *pConfig )
{
    if ( pConfig == NULL ) {
        DBGLOG( "Invalid config." );
        return NULL;
    }

    /* create instance */
    EightButtonController *pSelf =
        (EightButtonController *)malloc( sizeof(EightButtonController) );
    if ( pSelf == NULL ) {
        ERRLOG( "Malloc failed." );
        return NULL;
    }

    Keyboard *pKeyboard = keyboard_Open( pConfig->pKeyboardPathname );
    if ( pKeyboard == NULL ) {
        ERRLOG( "Keyboard open failed." );
        free( pSelf );
        return NULL;
    }

    /* initialize instance */
    pSelf->base      = sBase;
    pSelf->running   = false;
    pSelf->pKeyboard = pKeyboard;
    pSelf->pCom      = pConfig->pCom;
    pSelf->keyConfig = pConfig->keyConfig;

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
