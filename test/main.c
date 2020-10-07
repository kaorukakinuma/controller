#include "eight_button_controller.h"
#include "controller.h"

#define PATH "/dev/input/event2"

int main( void )
{
    EightButtonConfig config;
    Controller *pController;
    pController = __new__EightButtonController( PATH, &config );
    pController->Start( pController );
    pController->Stop( pController );
    pController = __del__EightButtonController( pController );
    return 0;
}
