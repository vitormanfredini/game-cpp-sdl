# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- implement transitions between tiles with different textures
- implement different ways that enemies can move (at an angle, sine wav adding and subtracting from movement, zig zag, chaotic/random but in your overall direction, aim and charge, etc)
- choose side that enemies come from in the level script (or make them appear mostly in the direction that the mainChar is moving)
- destroy projectiles when they are significantly off screen
- avoid rendering stuff off screen
- separate .h and .cpp files
- no texture is being unloaded. implement a way of telling the TextureManager the texture's context (menu, game world, in-game UI, etc) so we can unload them without unloading textures that are being used.
- main char healthbar render is not taking into account the screen offset, possible resizing necessary, etc
- when firing a projectile, fire it from the center of the Character (instead of the base) and calculate its direction also from that position so it doesn't miss the target. 
- make health slowly recover
- make a weapon factory with prototype pattern
- bug: sometimes it doesn't find the closest enemy and fires at an enemy far away
- render enemies from bottom to top (so they don't stay in front of others even though they are in the back)

# Compiling and running

```bash
./build.sh
./build/SDL2Game
```
