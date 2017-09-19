#include "mylib.h"
#include "statelib.h"
#include "game_state_gameover.h"
#include "images/gameover.h"

int gameover_state() 
{
    draw_image_3(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gameover);
    
    if (KEY_DOWN(START)) {
        while (KEY_DOWN(START)) {}
        
        return GAME_STATE_PLAY;
    }

    return GAME_STATE_LOOP; 
}
