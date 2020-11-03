#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <socket_com_client.h>
#include <socket_com_server.h>
#include <eight_button_controller.h>
#include <controller.h>

static const char * const PATHNAME = "/dev/input/event2";
static const char * const ADDRESS  = "127.0.0.1";
static const uint16_t PORT = 8888;

typedef struct {
    Com        *pCom;
    Controller *pController;
} Context;

static void * StartController( void *pArg )
{
    Context *pSelf = (Context *)pArg;

    EightButtonControllerConfig config = {
        .pKeyboardPathname = PATHNAME,
        .pCom              = __new__SocketComServer( PORT ),
        .keyConfig         = {
            .a     = KEY_M,
            .b     = KEY_K,
            .x     = KEY_J,
            .y     = KEY_I,
            .right = KEY_S,
            .left  = KEY_A,
            .up    = KEY_W,
            .down  = KEY_Z,
        },
    };
    Controller *pController = __new__EightButtonController( &config );

    pController->Start( pController );

    pSelf->pCom = config.pCom;
    pSelf->pController = pController;

    pthread_exit( NULL );
}

static void StopController( Context *pSelf )
{
    Com *pCom = pSelf->pCom;
    Controller *pController = pSelf->pController;

    pController->Stop( pController );

    pController = __del__EightButtonController( pController );
    pCom = __del__SocketComServer( pCom );
}

int main( void )
{
    Context self;

    pthread_t thread;
    pthread_create( &thread, NULL, StartController, &self );
    pthread_detach( thread );
    sleep( 1 );

    Com *pCom = __new__SocketComClient( ADDRESS, PORT );
    pCom->Open( pCom );

    while ( 1 ) {
        EightButtonControllerData data;
        memset( &data, 0, sizeof(data) );

        pCom->Read( pCom, data.aBulk, sizeof(data) );

        int i;
        for ( i = 0; i < 8; i++ ) {
            printf( "%d ", data.aBulk[i] );
        }
        printf( "\n" );

        if ( data.field.a == 1 ) break;
    }

    pCom->Close( pCom );
    pCom = __del__SocketComClient( pCom );

    StopController( &self );

    return 0;
}
