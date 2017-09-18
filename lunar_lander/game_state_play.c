#include "mylib.h"
#include "stdlib.h"
#include "statelib.h"
#include "game_state_play.h"
#include "images/pause.h"
#include "images/rocket.h"
#include "images/rocketfire.h"
#include "images/terrain.h"
#include "images/life.h"
#include "images/safelanding.h"
#include "images/crash.h"
#include "images/collision.h"

// Crashed
#define PLAY_TRANSITION_STATES 2
// Play
#define CRASHED_TRANSITION_STATES 1
#define LANDED_TRANSITION_STATES 1

void buildTerrainAndAssignEdges();
void drawTerrain();
void checkForCollisions();
int translateCrashedReasonToState();
void resetShip();
int didReset();
void moveShip();
// Drawing
void redraw();
void drawShip();
void drawFuel();
void drawLives();
// States
int runState();
int crashedState();
int landedState();
int clearState();

typedef enum CrashedReason {
    IMPACT_GROUND,
    IMPACT_GOAL,
    SAFE_LANDING
} CrashedReason;

/**
 * Represents a ship
 */
typedef struct Ship {
    int locX;
    int locY;
    int oldLocX;
    int oldLocY;
    int dirX;
    int speed;
    int height;
    int oldHeight;
    int width;
    char crashed;
    volatile int fuel;
    CrashedReason reason;
} Ship;
/**
 * Represents the player
 */
typedef struct Player {
    int lives;
    Ship ship;
} Player;
/*
Will have to give some thought to what to do if I ever wanted to have multiple 
or randomly generated maps
*/
EDGE edges[SCREEN_WIDTH];
Ship ship;
Player player;
/*
Not sure how to make this a part of the ship
*/
const u16 *currentShipImage;
const u16 *oldShipImage;
/**
 * Controls the playing state of the game.  There are sub states to this one 
 * but this essentially resets the game to the beginning.
 */
int playState() {
    State inGame;
    State crashed;
    State landed;
    State redraw;
    int exitStateIndex;
    
    player.lives = 3;
    player.ship = ship;

    State *inGameTransitions[] = { &crashed, &landed };
    State *crashedTransitions[] = { &redraw };
    State *landedTransitions[] = { &redraw };
    State *redrawTransitions[] = { &inGame };
    
    inGame = generateState(&runState, inGameTransitions);
    crashed = generateState(&crashedState, crashedTransitions);
    landed = generateState(&landedState, landedTransitions);
    redraw = generateState(&clearState, redrawTransitions);
    
    State currentState = inGame;
    ActionHandler currentAction = inGame.action;
    
    buildTerrainAndAssignEdges();
    resetShip();
    drawLives();
    
    while (player.lives) {
        
        // Reset button pressed
        if (didReset()) {
            // Reset
            return 1;
        }
        
        exitStateIndex = currentAction();

        if (!exitStateIndex) {
            continue;
        }

        resetShip();

        currentState = *(currentState.transitions[exitStateIndex - 1]);
        currentAction = currentState.action;

    }
    // Gameover
    return 2;
}

/**
 * Builds the traces for the terrain.  Collision detection will be based 
 * where the white and gold are rather however else it might be done.
 */
void buildTerrainAndAssignEdges() {
    drawTerrain();

    for (int r = 0; r < 40; r++) {
        for (int c = 0; c < 240; c++) {
            int terrainColor = terrain[r * 240 + c];
            // Only if this terrain has color
            if (terrainColor != 0x7FFF && terrainColor != 0x019C) {
                continue;
            }
            // Goal sits above other edges, don't overwrite it if it's marked
            if (edges[c].goal) {
                continue;
            }

            edges[c].x1 = c;
            edges[c].y1 = r + 120;
            edges[c].goal = terrainColor == 0x019C ? 1 : 0;
        }
    }
}
/**
 * Draws the image of the terrain.  The terrain is only 40 pixels in height 
 * and is only drawn at the bottom
 */
void drawTerrain() {
    // Draw Terrain
    drawImage3(120, 0, SCREEN_WIDTH, 40, terrain);
}

void checkForCollisions() {
    int checkX = ship.locX >> 8;
    int checkY = ship.locY >> 8;
    int width = ship.width;

    EDGE edgeX = edges[checkX];
    EDGE edgeWidth = edges[checkX + width];

    if ((edgeX.y1 == ((checkY) + 20)) || (edgeWidth.y1 == ((checkY) + 20))) {
        ship.crashed = 1;
        ship.reason = IMPACT_GROUND;
        if (edgeX.goal && edgeWidth.goal) { // Goal Check
            // Display win screen
            if (ship.speed > -75) {
                if (edgeX.y1 <= (checkY + 20) && edgeWidth.y1 <= checkY + 20)
                ship.reason = SAFE_LANDING;
            } else {
                ship.reason = IMPACT_GOAL;
            }   
        }
    }
}

int translateCrashedReasonToState() {
    switch (ship.reason) {
        case IMPACT_GROUND:
        case IMPACT_GOAL:
            return 1;
            break;
        case SAFE_LANDING:
            return 2;
            break;
        default:
            return 0;
            break;

    }
}
/**
 * Sets the ships parameters back to new state
 */
void resetShip() {
    ship.locX = 0x0500;
    ship.locY = 0x0F00;
    ship.dirX = 0x40;
    ship.speed = 0;
    ship.height = 20;
    ship.width = 13;
    ship.fuel = 0x0580;
    ship.crashed = 0;
}
/**
 * Checks to see if select was pressed which should restart the game
 */
int didReset() {
    return testForKey(SELECT, 1);
}
/**
 * Changes the location parameters of the ship so that it 'moves'
 */
void moveShip() {
    int speed = ship.speed;
    int fuel = ship.fuel;
    int dirX = ship.dirX;
    int height = ship.height;

    ship.oldHeight = ship.height;
    ship.oldLocX = ship.locX;
    ship.oldLocY = ship.locY;
    oldShipImage = currentShipImage;

    // Thrust
    if (KEY_DOWN(UP) && (fuel>0)) {
        currentShipImage = rocketfire;
        height = 26;
        speed += 0x0001;
        fuel -= 0x0002;
        if (speed >= 0x0100) {
            speed = 0x0100;
        }
    } else {
        currentShipImage = rocket;
        height = 20;
        speed -= 0x0001;
        if (speed < -0x0090) {
            speed = -0x0090;
        }
    }
    // Move Right
    if (KEY_DOWN(RIGHT) && fuel) {
        fuel -= 0x0001;
        dirX += 0x0007;
        if (dirX > 0x0050) {
            dirX = 0x0050;
        }
        
    } else if (dirX > 0) {
        dirX -= 0x0001;
        if (dirX < 0) {
            dirX = 0;
        }
    }
    // Move Left
    if (KEY_DOWN(LEFT) && fuel) {
        fuel -= 0x0001;
        dirX -= 0x0007;
        if (dirX <= -0x0050) {
            dirX = -0x0050;
        }
    } else if (dirX < 0) {
        dirX += 0x0001;
        if (dirX > 0) {
            dirX = 0;
        }
    }
    
    ship.speed = speed;
    ship.fuel = fuel;
    ship.height = height;
    ship.dirX = dirX;
    ship.locX += dirX;
    ship.locY -= speed;
}

void redraw() {
    waitForVBlank();

    drawTerrain();
    drawShip();
    drawLives();
    drawFuel();
}

void drawShip() {
    drawRect(
        (ship.oldLocY >> 8), 
        (ship.oldLocX >> 8), 
        ship.width, 
        ship.oldHeight, 
        black);

    drawImage3(
        (ship.locY >> 8), 
        (ship.locX >> 8), 
        ship.width, 
        ship.height, 
        currentShipImage);
}

void drawLives() {
    for (int a = 0; a < player.lives; a++) {
        drawImage3(140, (3 + (a) * 7), 7, 13, life);
    }
}

void drawFuel() {
    drawHollowRect(140, 140, 52, 12, COLOR(31, 31, 31));
    drawRect(141, 141, 50 + 1, 10, black);
    drawRect(141, 141, (10 * (ship.fuel >> 8)) + 1, 10, red);
}

int runState() {
    moveShip();
    redraw();
    // See about returning the collision check.
    checkForCollisions(ship, edges);

    if (!ship.crashed) {
        return 0;
    }

    if (ship.reason != SAFE_LANDING) {
        player.lives -= 1;
    }

    return translateCrashedReasonToState();
}
/**
 * Draws the appropriate picture for a crash.  There is technically no reason 
 * to continuously redraw this state or the landed state but I didn't find it 
 * problem.  Would still want to wait on vblank
 */
int crashedState() {
    drawImage3(
        10, 
        0, 
        SCREEN_WIDTH, 
        50, 
        (ship.reason == IMPACT_GOAL ? collision : crash));

    return testForKey(START, 1);
}

int landedState() {
    drawImage3(10, 0, SCREEN_WIDTH, 50, safelanding);

    return testForKey(START, 1);
}

int clearState() {
    drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, black);
    redraw();

    return 1;
}

