#include "mylib.h"
#include "statelib.h"
#include "game_state.h"
#include "game_state_init.h"
#include "game_state_play.h"
#include "game_state_gameover.h"
#include <stdlib.h>

#define INIT_TRANSITION_STATES 1
#define PLAY_TRANSITION_STATES 2
#define GAMEOVER_TRANSITION_STATES 1

void init_game() {
    int exitStateIndex;

    State init;
    State play;
    State gameover;
    
    // Initialize Display
    REG_DISPCNT = (MODE3 | BG2_ENABLE);

    State *initTransitions[] = { &play };
    State *playTransitions[] = { &play, &gameover };
    State *gameoverTransitions[] = { &play };

    init = generateState(&initState, initTransitions);
    play = generateState(&playState, playTransitions);
    gameover = generateState(&gameoverState, gameoverTransitions);
    
    State currentState = init;

    while(1) {
        waitForVBlank();    
        exitStateIndex = currentState.action();

        if (!exitStateIndex) {
            continue;
        }

        waitForVBlank();    
        // Most of the transitions will require the screen be cleared.
        drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, black);

        currentState = *(currentState.transitions[exitStateIndex - 1]);
    }
}
