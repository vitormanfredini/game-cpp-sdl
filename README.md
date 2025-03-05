# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- draw healthbar on top of each Character
- choose side that enemies come from in the level script
- load a config json for characters (for specifying base attributes for size, velocity, texture, etc) so we can have many types.
- destroy projectiles when they are significantly off screen
- avoid rendering stuff off screen
- separate .h and .cpp files
- no texture is being deleted
- healthbar render is not taking into account the screen offset, possible resizing necessary, etc
- implement different ways that enemies can move (at an angle, sine wav adding and subtracting from movement, zig zag, chaotic/random but in your overall direction, aim and charge, etc)
- when firing a projectile, fire it from the center of the Character (instead of the base) and calculate its direction also from that position so it doesn't miss the target. 

# Compiling and running

```bash
./build.sh
./build/SDL2Game
```
