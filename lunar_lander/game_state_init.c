#include "mylib.h"
#include "statelib.h"
#include "game_state_init.h"
#include "images/splash.h"

int init_state(void) 
{
    draw_image_3(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, splash);

    if (KEY_DOWN(START)) {
        while(KEY_DOWN(START)) {}

        return GAME_STATE_PLAY;
    }

    return GAME_STATE_LOOP;
}
