## The Game

A poor mans Lunar Lander...

It's playable with the TONC gameboy emulator but the version I used had a 
requirement of Ubuntu 12.x as APT in newer versions didn't have the right 
libraries easily available.

It has been a while since I wrote this but it uses MODE3.  This gives each 
pixel the full space for most of the colors I would need.  I think this choice 
was purely due to expediency.  If I had the knowledge fresher in my head I 
would have switched to a different mode that allowed page flipping.  I 
think this would help with at least one of the redraws so I that could avoid 
the slight bar of tearing that occurs as a result.  And it might be fun to 
utilize more of the things that the gameboy provides with layering and such.

Drawing the surface directly on the board, though, allowed me to more easily do 
simple collision detection storing it as an array of Edge structs.  The colors 
provide goal markers to determine if the ship is in the right place on impact.

There are a few magic numbers in the draw functions but I believe they're 
mostly harmless.  And the graphics are definitely not the work of a designer
or anyone having any semblence of artistic ability whatsoever.  **And there is 
still only one map.**

Ideally the code quality will prove to be easily readable and mostly well 
architected.  This game started life as a fairly long, 4-500 lines, chunk of 
if/else statements so I believe this is a vast improvement in terms of 
readability.

### Controls

```
Start = ENTER

Up, Down, Left, Right = ARROW KEYS
```

The idea is to _safely_ land on the orange/golden platforms.  Where _safely_ 
means somewhat slower that terminal velocity.

## Build
 
Change directory to `lunar_lander`

```bash
make vba
```

Does not work using the `make wxvba` flag.  Had not previously tested it...

## State machine

It operates based on two state machines:

The outter loop that shows the start screen, transitions to a playing state and 
moves to the gameover screen onces the player is out of lives.  This outter loop has 
no control once the inner play state is entered; it issimply waiting.

The transition occurs when a state exits with something other than zero. Zero 
simply causes the state to play over again for most states.  The transitions 
are based on the location of the state in transitions array offset by 1.

So a return of `1` from any state would cause it to transition to the state in 
the `0` index of that states transition array.

### The outer loop

**init** -> **ingame** <--> **gameover**

Where play has it's own inner loop that moves between the various in game 
states

### The inner loop (ingame)

If player has lives at end of crash

```
**play** -> **crashed** -> **redraw**
   ^                            |
   |----------------------------|
```

If player has no lives at the end of crash
```
**play** -> **crashed** -> exit to outter loop
```
If player lands safely
```
**play** -> **landed**  -> **redraw**
    ^                           |
    |---------------------------|
```