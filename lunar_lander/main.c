#include "game_state.h"
// Asa Freedman


int main() {
    init_game();

    return 0;
}
    
// drawRect(0,0,240,160,black); // Cover splash
// // Draw Terrain
// drawImage3(120,0,240,40,terrain);
// // Draw Fuel Gauge

// // Draw Lives
// for (int a = 0; a < lives; a++) {
//     drawImage3(140,(3 + a*7),7,13,life);
// }
// // Draw Rocket
// while(1) {
//     oldx = x >> 8;
//     oldy = y >> 8;
    
//     // PAUSE SCREEN
//     if (KEY_DOWN(START) && !startpressed) {
//         drawImage3(20,0,240,50,pause);
//         while (KEY_DOWN(START)) {
//             startpressed = 0;
//         }
//         while (!startpressed) {
//             while (KEY_DOWN(START)) {
//                 startpressed = 1;
//             }
//         }
//         drawRect(20,0,240,50,black);
//     } else startpressed = 0;
    
//     if (KEY_DOWN(SELECT)) {
//         while (KEY_DOWN(SELECT)) {}
//         reset = 1;
//         selectpressed = 1;
//     }
    
//     if (crashed) {
//         reset = 1;
//         fuel += 0x0300;
//         if (fuel > 0x0580) {
//             fuel = 0x0580;
//         }
//     }
    
//     if (reset) {
//         // Reset due to bad landing
//         if (!landedsafely) {
//             lives -= 1;
//         }
//         // Reset due to select
//         if (selectpressed) {        
//             drawImage3(0,0,240,160,splash);
//             while (!startpressed) {
//                 if (KEY_DOWN(START)) {
//                     startpressed = 1;
//                 }
//             }
//             drawRect(0,0,240,160,black); // Cover splash
//             lives = 3;
//             fuel = 0x0580;
//             selectpressed = 0;
//         }
//         dirx = 0x40;
//         x = 0x0500;
//         y = 0x0F00;
//         width = 13;
//         height = 20;
//         speed = 0;
//         // Game over due to loss of lives
//         if (lives<0) { // Out of lives - gameover
//             drawImage3(0,0,240,160,gameover);
//             while (!startpressed) {
//                 while (KEY_DOWN(START)) {
//                     startpressed = 1;
//                     lives = 3;
//                     fuel = 0x0580;
//                 }        
//             }
//             // Clear Game Screen
//             drawRect(0,0,240,160,black);
//         } else if (landedsafely) { // 
//             drawImage3(10,0,240,50,safelanding);
//             while (!startpressed) {
//                 while (KEY_DOWN(START)) {
//                     startpressed = 1;
//                 }
//             }
//         } else if (collide) { // Landed on the pad but too fast
//             drawImage3(10,0,240,50,collision);
//             while (!startpressed) {
//                 while (KEY_DOWN(START)) {
//                     startpressed = 1;
//                 }
//             }
//         } else if (crashed) { // Crash on surface
//             drawImage3(10,0,240,50,crash);
//             while (!startpressed) {
//                 while (KEY_DOWN(START)) {
//                     startpressed = 1;
//                 }
//             }
//         }
//         drawRect(10,0,240,160,black);
//         collide = 0;
//         crashed = 0;
//         landedsafely = 0;
//         drawRect(oldy,oldx,width,height, black);
//         // Redraw Terrain
//         drawImage3(120,0,240,40,terrain);
//         // Draw Lives
//         drawRect(140,(3+lives)*7,7,13,black);
//         for (int a = 0; a < lives; a++) {
//             drawImage3(140,(3 + a*7),7,13,life);
//         }
//     }
//     // Redraw the old ship before possible image change
//     if (!crashed) drawRect(oldy-1,oldx-1,width+2,height+2, black);
//     // INCREASE THRUST
//     if (KEY_DOWN(UP) && (fuel>0)) {
//         speed += 0x0001;
//         fuel -= 0x0002;
//         image = rocketfire;
//         height = 26;
//         if (speed >= 0x0100) {
//             speed = 0x0100;
//         }
//     } else {
//         speed -= 0x0001;
//         image = rocket;
//         height = 20;
//         if (speed < -0x0090) {
//             speed = -0x0090;
//         }
//     }
//     // VECTOR RIGHT 
//     if (KEY_DOWN(RIGHT) && fuel) {
//         fuel -= 0x0001;
//         dirx += 0x0007;
//         if (dirx > 0x0050) {
//             dirx = 0x0050;
//         }
        
//     } else if (dirx > 0) {
//         dirx -= 0x0001;
//         if (dirx < 0) {
//             dirx = 0;
//         }
//     }
//     // VECTOR LEFT
//     if (KEY_DOWN(LEFT) && fuel) {
//         fuel -= 0x0001;
//         dirx -= 0x0007;
//         if (dirx <= -0x0050) {
//             dirx = -0x0050;
//         }
//     } else if (dirx < 0) {
//         dirx += 0x0001;
//         if (dirx > 0) {
//             dirx = 0;
//         }
//     }
    
//     x += dirx;      // Changing the left and right directions
//     y -= speed;     // Force due to gravity neglecting vector forces
//     // CHECK FOR COLLISIONS
    
//     int checkx = x>>8;
//     int checky = y>>8;
//     if (fuel < 0) fuel = 0; // Make sure I don't send a 0 to DMA
//     if (checkx < 0x0000 || (checkx+width) > 240) {
//         dirx = -dirx;
//     }
//     if ((edges[checkx].y1 == ((checky)+20)) || (edges[checkx + width].y1 == ((checky)+20))) {
//         if (edges[checkx].goal && edges[checkx+width].goal) { // Goal Check
//             // Display win screen
//             if (speed > -75) {
//                 if (edges[checkx].y1 <= (checky+20) && edges[checkx+width].y1 <= checky+20)
//                 landedsafely = 1;
//             } else {
//                 collide = 1;
//             }   
//         }
//         crashed = 1;
//     } 
//     if (reset) { // Takes care of reset terrain glitch
//         drawImage3(120,0,240,40,terrain);
//         for (int a = 0; a < lives; a++) {
//             drawImage3(140,(3 + (a)*7),7,13,life);
//         }
//         reset = 0;
//     }
    
//     // Only redraw terrain if you're in the possible terrain range
//     if (((checky) + height) > 120) {
//         // Redraw Terrain
//         drawImage3(120,0,240,40,terrain);
//         // Redraw Lives
//         for (int a = 0; a < lives; a++) {
//             drawImage3(140,(3 + (a)*7),7,13,life);
//         }
//     }
//     // Draw Fuel Gauge
//     drawHollowRect(140, 140, 52, 12, COLOR(31,31,31));
//     drawRect(141,141,50+1,10,black);
//     drawRect(141,141,(10*(fuel>>8))+1,10,red);
//     // Draw ship here
    
    
//     drawImage3((checky),(checkx),width, height, image);
//     }
//     return 0;
// }

