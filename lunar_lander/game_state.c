#include "mylib.h"
#include "game_state.h"
#include "images/splash.h"
#include "images/gameover.h"
#include "images/pause.h"
#include "images/rocket.h"
#include "images/rocketfire.h"
#include "images/terrain.h"
#include "images/life.h"
#include "images/safelanding.h"
#include "images/crash.h"
#include "images/collision.h"
#include <debugging.h>
#include <stdlib.h>

#define INIT_TRANSITION_STATES 1
#define PLAY_TRANSITION_STATES 2
#define GAMEOVER_TRANSITION_STATES 1

int initState(void);
int playState(void);
int gameoverState(void);

typedef int (*ActionHandler)(void);

typedef struct State State;

struct State {
    ActionHandler action;
    State **transitions;
};

typedef struct {
    State currentState;
} GameState;

void init_game() {
    int exitStateIndex;

    State init;
    State play;
    State gameover;
    
    // Initialize Display
    REG_DISPCNT = (MODE3 | BG2_ENABLE);
    
    init.action = &initState;
    // init.transitions[0] = &play;
    init.transitions 
        = (State **)malloc(sizeof(State *) * INIT_TRANSITION_STATES);
    // Pressed start
    init.transitions[0] = &play;

    play.action = &playState;
    play.transitions 
        = (State **)malloc(sizeof(State *) * PLAY_TRANSITION_STATES);
    // Reset
    play.transitions[0] = &play;
    // Out of lives, crashed, whatever
    play.transitions[1] = &gameover;

    gameover.action = &gameoverState;
    gameover.transitions 
        = (State **)malloc(sizeof(State *) * GAMEOVER_TRANSITION_STATES);
    // User presses start to restart the game
    gameover.transitions[0] = &play;

    GameState gameState = { .currentState = init };

    while(1) {
        waitForVBlank();    
        exitStateIndex = gameState.currentState.action();

        if (!exitStateIndex) {
            continue;
        }

        // Most of the transitions will require the screen be cleared.
        drawRect(0, 0, 240, 160, black);

        gameState.currentState 
            = *(gameState.currentState.transitions[exitStateIndex - 1]);
    }
}

int initState(void) {
    // Draw Splash Screen
    drawImage3(0, 0, 240, 160, splash);

    while(KEY_DOWN(START)) {
        return 1;
    }

    return 0;
}

int playState() {
    // // Game Settings
    // //int gameover = 0;
    // int lives = 3;
    // int selectpressed = 0;
    // int collide = 0;
    // int reset = 0;
    // // Player Piece
    // int dirx = 0x40;
    // int x = 0x0500;
    // int oldx = x;
    // int y = 0x0F00;
    // int oldy = y;
    // int width = 13;
    // int height = 20;
    // int speed = 0;
    // volatile int fuel = 0x0580;
    // int crashed = 0;
    // int landedsafely = 0;
    // volatile u16 black = 0;
    // volatile u16 red = (COLOR(31, 0, 0));
    // // Images
    // const u16* image = rocket;
    // // Initialize the vertices tracing through terrain
    // EDGE edges[240];
    // for (int r = 0; r < 40; r++) {
    //     for (int c = 0; c < 240; c++) {
    //         if ((terrain[r*240+c] == 0x7FFF) && (!edges[c].goal)) {
    //             edges[c].x1 = c;
    //             edges[c].y1 = r+120;
    //             edges[c].goal = 0;
    //         } else if (terrain[r*240+c] == 0x019c) {
    //             edges[c].x1 = c;
    //             edges[c].y1 = r + 120;
    //             edges[c].goal = 1;
    //         }
    //     }
    // }

    return 0;
}

int gameoverState() {
    drawImage3(0, 0, 240, 160, gameover);
    
    while (KEY_DOWN(START)) {
        return 1;
    }

    return 0; 
}