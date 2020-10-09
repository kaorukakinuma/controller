#include <eight_button_controller.h>
#include <controller.h>

#include <socket_com_client.h>

static const char * const PATHNAME = "/dev/input/event2";
static const char * const ADDRESS  = "127.0.0.1";
static const uint16_t PORT = 0;

int main( void )
{
    Com *pCom = __new__SocketComClient( ADDRESS, PORT );

    EightButtonControllerKeyConfig keyConfig = {
        .a     = KEY_M,
        .b     = KEY_K,
        .x     = KEY_J,
        .y     = KEY_I,
        .right = KEY_S,
        .left  = KEY_A,
        .up    = KEY_W,
        .down  = KEY_Z,
    };
    EightButtonControllerConfig config = {
        .pKeyboardPathname = PATHNAME,
        .pCom              = pCom,
        .keyConfig         = keyConfig
    };

    Controller *pController = __new__EightButtonController( &config );

    pController->Start( pController );
    // snip
    pController->Stop( pController );

    pController = __del__EightButtonController( pController );
    pCom = __del__SocketComClient( pCom );

    return 0;
}
