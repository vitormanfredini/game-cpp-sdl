# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- create everything using smart pointers and allow GameEngine to manage their lifetime
- choose side that enemies come from in the level script
- make enemies collide and push each other (dont let them stack on the screen)
- load a config json for characters (for specifying base attributes for size, velocity, texture, etc) so we can have many types.
- getMovementDirectionTowards has a limitation where diagonals get a different speed than vertical/horizontal because of the way it's calculated
- destroy projectiles when they are significantly off screen
- avoid rendering stuff off screen
- separate .h and .cpp files
- think about using a time factor instead of fixed time update (probably not)
- no texture is being deleted
- healthbar render is not taking into account the screen offset, possible resizing necessary, etc
- implement different ways that enemies can move (at an angle, sine wav adding and subtracting from movement, zig zag, chaotic/random but in your overall direction, aim and charge, etc)
- healthbar on top of each Character
- consider the X and Y the bottom center of the Character

# Compiling and running

```bash
./build.sh
./build/SDL2Game
```
