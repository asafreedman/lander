#include "mylib.h"
#include "statelib.h"
#include "game_state.h"
#include "game_state_init.h"
#include "game_state_play.h"
#include "game_state_gameover.h"
#include <stdlib.h>

void init_game() 
{
    int exit_state_index;

    State init;
    State play;
    State gameover;
    
    // Initialize Display
    REG_DISPCNT = (MODE3 | BG2_ENABLE);

    State *init_transitions[] = { &play };
    State *play_transitions[] = { &play, &gameover };
    State *gameover_transitions[] = { &play };

    init = generate_state(&init_state, init_transitions);
    play = generate_state(&play_state, play_transitions);
    gameover = generate_state(&gameover_state, gameover_transitions);
    
    State current_state = init;

    while(1) {
        wait_for_v_blank();    
        exit_state_index = current_state.action();

        if (!exit_state_index) {
            continue;
        }

        wait_for_v_blank();    
        // Most of the transitions will require the screen be cleared.
        draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, black);

        current_state = *(current_state.transitions[exit_state_index - 1]);
    }
}
