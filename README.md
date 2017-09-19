A poor mans Lunar Lander...

It's playable with the TONC gameboy emulator but the version I used had a 
requirement of Ubuntu 12.x as APT in newer versions didn't have the right 
libraries available.

It has been a while since I wrote this but it uses MODE3.  This gives each 
pixel the full space for most of the colors I would need.  I think this choice 
was purely due to expediency.  If I had the knowledge fresher in my head I 
would have switched to a different mode that allowed page flipping.  I 
think this would help with at least one of the redraws so I that could avoid 
the slight bar of tearing that occurs as a result.

There are a few magic numbers in the draw functions but I believe they're 
mostly harmless.

## The Game

Start = ENTER
Up, Down, Left, Right = ARROW KEYS

The idea is to _safely_ land on the orange/golden platforms.  Where safely 
means somewhat slower that max velocity.

## State machine

It operates based on two state machines:

An outter loop that shows the start screen, transitions to a playing state and 
moving to the gameover screen onces the lives are over.  This outter loop has 
no control once the inner play state is entered, simply waiting.

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