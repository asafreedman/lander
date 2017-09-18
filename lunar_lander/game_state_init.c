#include "game_state_init.h"
#include "mylib.h"
#include "images/splash.h"

int initState(void) {
    drawImage3(0, 0, 240, 160, splash);

    if (KEY_DOWN(START)) {
        while(KEY_DOWN(START)) {}

        return 1;
    }

    return 0;
}
