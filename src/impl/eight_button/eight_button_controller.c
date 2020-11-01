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
#define _DEFAULT_SOURCE

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include <keyboard.h>

#include "eight_button_controller.h"
#include "controller_log.h"
#include "controller.h"

typedef struct {
    Controller                  base;
    bool                        running;
    pthread_t                   thread;
    Keyboard                   *pKeyboard;
    Com                        *pCom;
    EightButtonControllerKey    keyConfig;
} EightButtonController;

/* ------------------------------------------------------------------------- */

static void * MainThread( void *pArg )
{
    EightButtonController *pSelf = (EightButtonController *)pArg;

    Keyboard *pKeyboard = pSelf->pKeyboard;
    Com *pCom = pSelf->pCom;
    EightButtonControllerKey keyConfig = pSelf->keyConfig;

    pCom->Open( pCom );

    while ( pSelf->running ) {
        struct timespec delay = { .tv_sec = 0, .tv_nsec = 10*1000*1000 };
        nanosleep( &delay, NULL );

        EightButtonControllerData data;
        data.field.a     = keyboard_GetKeyState( pKeyboard, keyConfig.a );
        data.field.b     = keyboard_GetKeyState( pKeyboard, keyConfig.b );
        data.field.x     = keyboard_GetKeyState( pKeyboard, keyConfig.x );
        data.field.y     = keyboard_GetKeyState( pKeyboard, keyConfig.y );
        data.field.right = keyboard_GetKeyState( pKeyboard, keyConfig.right );
        data.field.left  = keyboard_GetKeyState( pKeyboard, keyConfig.left );
        data.field.up    = keyboard_GetKeyState( pKeyboard, keyConfig.up );
        data.field.down  = keyboard_GetKeyState( pKeyboard, keyConfig.down );

        pCom->Write( pCom, data.aBulk, sizeof(data) );
    }

    pCom->Close( pCom );

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

static int CheckConfig( EightButtonControllerConfig *pConfig )
{
    if ( pConfig == NULL ) {
        DBGLOG( "Invalid config." );
        return -1;
    }
    if ( pConfig->pKeyboardPathname == NULL ) {
        DBGLOG( "Invalid keyboard pathname." );
        return -1;
    }
    if ( pConfig->pCom == NULL ) {
        DBGLOG( "Invalid com." );
        return -1;
    }
    return 0;
}

Controller * __new__EightButtonController( EightButtonControllerConfig *pConfig )
{
    if ( CheckConfig(pConfig) < 0 ) {
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
