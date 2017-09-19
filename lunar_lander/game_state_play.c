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
// Decreasing my stock of magic numbers
#define STARTING_PLAYER_LIVES 3
#define MAX_DOWNWARD_VELOCITY_TO_LAND -75
#define TERRAIN_DRAW_START_Y 120
#define BANNER_DRAW_START_Y 10
// In order of appearance
void build_terrain_and_assign_edges();
void draw_terrain();
void check_for_collision();
int translate_crashed_reason_to_state();
void reset_ship();
int did_reset();
void move_ship();
// Drawing
void redraw();
void draw_ship();
void draw_fuel();
void draw_lives();
// States
int run_state();
int crashed_state();
int landed_state();
int clear_state();

typedef enum CrashedReason {
    IMPACT_GROUND,
    IMPACT_GOAL,
    SAFE_LANDING
} CrashedReason;
/**
 * Represents a ship
 */
typedef struct Ship {
    int loc_x;
    int loc_y;
    int old_loc_x;
    int old_loc_y;
    int dir_x;
    int speed;
    int height;
    int old_height;
    int width;
    int fuel;
    char crashed;
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
const u16 *current_ship_image;
const u16 *old_ship_image;
/**
 * Controls the playing state of the game.  There are sub states to this one 
 * but this essentially resets the game to the beginning.
 */
int play_state() 
{
    State in_game;
    State crashed;
    State landed;
    State clear;
    int exit_state_index;
    
    player.lives = STARTING_PLAYER_LIVES;
    player.ship = ship;

    State *in_game_transitions[] = { &crashed, &landed };
    State *crashed_transitions[] = { &clear };
    State *landed_transitions[] = { &clear };
    State *clear_transitions[] = { &in_game };
    
    in_game = generate_state(&run_state, in_game_transitions);
    crashed = generate_state(&crashed_state, crashed_transitions);
    landed = generate_state(&landed_state, landed_transitions);
    clear = generate_state(&clear_state, clear_transitions);
    
    State current_state = in_game;
    ActionHandler current_action = in_game.action;
    
    build_terrain_and_assign_edges();
    reset_ship();
    draw_lives();
    
    while (player.lives) {
        
        // Reset button pressed
        if (did_reset()) {
            // Reset
            return GAME_STATE_PLAY;
        }
        
        exit_state_index = current_action();

        if (!exit_state_index) {
            continue;
        }

        reset_ship();

        current_state = *(current_state.transitions[exit_state_index - 1]);
        current_action = current_state.action;

    }
    // Gameover
    return GAME_STATE_GAMEOVER;
}

/**
 * Builds the traces for the terrain.  Collision detection will be based 
 * where the white and orange are rather however else it might be done.
 */
void build_terrain_and_assign_edges() 
{
    draw_terrain();

    for (int r = 0; r < TERRAIN_HEIGHT; r++) {
        for (int c = 0; c < SCREEN_WIDTH; c++) {
            int terrain_color = terrain[r * SCREEN_WIDTH + c];
            // Only if this terrain has color
            if (terrain_color == black) {
                continue;
            }
            // Goal sits above other edges, don't overwrite it if it's marked
            if (edges[c].goal) {
                continue;
            }

            edges[c].x1 = c;
            edges[c].y1 = r + TERRAIN_DRAW_START_Y;
            edges[c].goal = terrain_color == orange ? 1 : 0;
        }
    }
}
/**
 * Draws the image of the terrain.  The terrain is only 40 pixels in height 
 * and is only drawn at the bottom
 */
void draw_terrain() 
{
    // Draw Terrain
    draw_image_3(
        TERRAIN_DRAW_START_Y, 
        0, 
        SCREEN_WIDTH, 
        TERRAIN_HEIGHT, 
        terrain);
}

void check_for_collision() 
{
    int checkX = ship.loc_x >> 8;
    int checkY = ship.loc_y >> 8;
    int width = ship.width;

    EDGE edge_x = edges[checkX];
    EDGE edge_width = edges[checkX + width];

    if ((edge_x.y1 == ((checkY) + 20)) || (edge_width.y1 == ((checkY) + 20))) {
        ship.crashed = 1;
        ship.reason = IMPACT_GROUND;
        // Within the bounds of a goal
        if (edge_x.y1 <= (checkY + 20) && edge_width.y1 <= checkY + 20) {
            // Display win screen
            if (ship.speed > MAX_DOWNWARD_VELOCITY_TO_LAND) {
                ship.reason = SAFE_LANDING;
            } else {
                ship.reason = IMPACT_GOAL;
            }   
        }
    }
}

int translate_crashed_reason_to_state() 
{
    switch (ship.reason) {
        case IMPACT_GROUND:
        case IMPACT_GOAL:
            return PLAY_STATE_CRASHED;
            break;
        case SAFE_LANDING:
            return PLAY_STATE_LANDED;
            break;
        default:
            return PLAY_STATE_LOOP;
            break;

    }
}
/**
 * Sets the ships parameters back to new state
 */
void reset_ship() 
{
    ship.loc_x = 0x0500;
    ship.loc_y = 0x0F00;
    ship.dir_x = 0x40;
    ship.speed = 0;
    ship.height = 20;
    ship.width = 13;
    ship.fuel = 0x0580;
    ship.crashed = 0;
}
/**
 * Checks to see if select was pressed which should restart the game
 */
int did_reset() 
{
    return test_for_key(SELECT, 1);
}
/**
 * Changes the location parameters of the ship so that it 'moves'
 */
void move_ship() 
{
    int speed = ship.speed;
    int fuel = ship.fuel;
    int dir_x = ship.dir_x;
    int height = ship.height;

    ship.old_height = ship.height;
    ship.old_loc_x = ship.loc_x;
    ship.old_loc_y = ship.loc_y;
    old_ship_image = current_ship_image;

    // Thrust
    if (KEY_DOWN(UP) && (fuel>0)) {
        current_ship_image = rocketfire;
        height = 26;
        speed += 0x0001;
        fuel -= 0x0002;
        if (speed >= 0x0100) {
            speed = 0x0100;
        }
    } else {
        current_ship_image = rocket;
        height = 20;
        speed -= 0x0001;
        if (speed < -0x0090) {
            speed = -0x0090;
        }
    }
    // Move Right
    if (KEY_DOWN(RIGHT) && fuel) {
        fuel -= 0x0001;
        dir_x += 0x0007;
        if (dir_x > 0x0050) {
            dir_x = 0x0050;
        }
        
    } else if (dir_x > 0) {
        dir_x -= 0x0001;
        if (dir_x < 0) {
            dir_x = 0;
        }
    }
    // Move Left
    if (KEY_DOWN(LEFT) && fuel) {
        fuel -= 0x0001;
        dir_x -= 0x0007;
        if (dir_x <= -0x0050) {
            dir_x = -0x0050;
        }
    } else if (dir_x < 0) {
        dir_x += 0x0001;
        if (dir_x > 0) {
            dir_x = 0;
        }
    }
    
    ship.speed = speed;
    ship.fuel = fuel;
    ship.height = height;
    ship.dir_x = dir_x;
    ship.loc_x += dir_x;
    ship.loc_y -= speed;
}

void redraw() 
{
    wait_for_v_blank();

    draw_terrain();
    draw_ship();
    draw_lives();
    draw_fuel();
}
/**
 * Draws the ship and a box over the old ships location... Shoudl have used 
 * mode 2
 */
void draw_ship() 
{
    draw_rect(
        (ship.old_loc_y >> 8), 
        (ship.old_loc_x >> 8), 
        ship.width, 
        ship.old_height, 
        black);

    draw_image_3(
        (ship.loc_y >> 8), 
        (ship.loc_x >> 8), 
        ship.width, 
        ship.height, 
        current_ship_image);
}
/**
 * Draw the player lives remaining 
 */
void draw_lives() 
{
    for (int a = 0; a < player.lives; a++) {
        draw_image_3(140, (3 + (a) * 7), 7, 13, life);
    }
}
/**
 * Draw the fuel gauge
 */
void draw_fuel() 
{
    draw_hollow_rect(140, 140, 52, 12, COLOR(31, 31, 31));
    draw_rect(141, 141, 51, 10, black);
    draw_rect(141, 141, (10 * (ship.fuel >> 8)) + 1, 10, red);
}

int run_state() 
{
    move_ship();
    redraw();
    // See about returning the collision check.
    check_for_collision(ship, edges);

    if (!ship.crashed) {
        return PLAY_STATE_LOOP;
    }

    if (ship.reason != SAFE_LANDING) {
        player.lives--;
    }
    // I could return check_for_collision but I have other things to do after
    return translate_crashed_reason_to_state();
}
/**
 * Draws the appropriate picture for a crash.  There is technically no reason 
 * to continuously redraw this state or the landed state but I didn't find it 
 * problem.  Would still want to wait on vblank
 */
int crashed_state() 
{
    draw_image_3(
        BANNER_DRAW_START_Y, 
        0, 
        SCREEN_WIDTH, 
        COLLISION_HEIGHT, 
        (ship.reason == IMPACT_GOAL ? collision : crash));

    return test_for_key(START, PLAY_STATE_CLEAR);
}

int landed_state() 
{
    draw_image_3(
        BANNER_DRAW_START_Y, 
        0, 
        SCREEN_WIDTH, 
        SAFELANDING_HEIGHT, 
        safelanding);

    return test_for_key(START, PLAY_STATE_CLEAR);
}

int clear_state() 
{
    draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, black);
    redraw();

    return PLAY_STATE_PLAY;
}

