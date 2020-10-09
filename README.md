# Controller

Dependencies: `lkeyboard` `lcom` `lpthread` `lrt`  
Includes: `controller.h` `eight_button_controller.h`


## How to Build

```sh
$ sudo ./mk.sh
#snip
$ ls /home/lib
libcontroller.a
$ ls /home/include
controller.h ...
```


## Design

```plantuml
@startuml

class Controller << (I, yellow) interface >> {
    + Start()
    + Stop()
}
namespace CONTROLLER {
    class EightButtonController << (M, orange) >> {
        + constructor()
        + destructor()
    }
}

Controller <|. CONTROLLER : <<implements>>

@enduml
```


## Usage

```c
#include <eight_button_controller.h>
#include <controller.h>

static const char * const PATHNAME = "/dev/input/event2";
static const char * const ADDRESS  = "127.0.0.1";
static const uint16_t PORT = 0;

int main( void )
{
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
    Com *pCom = __new__SocketComClient( ADDRESS, PORT );

    EightButtonControllerConfig config = {
        .pKeyboardPathname = PATHNAME,
        .pCom              = pCom;
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
```
