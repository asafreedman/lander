#include "mylib.h"
#include "game_state_gameover.h"
#include "images/gameover.h"

int gameoverState() {
    drawImage3(0, 0, 240, 160, gameover);
    
    if (KEY_DOWN(START)) {
        while (KEY_DOWN(START)) {}
        
        return 1;
    }

    return 0; 
}