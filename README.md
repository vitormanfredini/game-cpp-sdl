# SDL Game

This is a project I'm doing for studying C++ and SDL.

UPGRADES:
vida aumenta X a cada X tempos
FEITO vida máxima
aumentar a velocidade base

ITEM:
mais rápido por um tempo limitado

TODO:
- loadTextureFromText: move most of the code to SDUtils in a way that a text can be loaded to a new transparent texture or written onto an existing texture
- MenuFactory: write upgrade name on option button (and add support to render text on textures)
- MenuFactory edge case: when no upgrades are available. (implement regular items to replace the upgrades)
- MenuFactory: implement a way to calculate buttons sizes and positions base an area for them to fill.
- move MainChar settings to CharacterFactory
- implement transitions between tiles with different textures
- implement different ways that enemies can move: sine wav adding and subtracting from movement, aim and charge, etc
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

# Rendering specifics

The renderer accepts virtual values and maps them onto the screen. If the screen is a perfect square (for example, 500x500) then 0.0f,0.0f is exactly on the top left corner and 1.0fx1.0f is exactly on the bottom right corner.
But when the screen isn't a perfect square, it scales the virtual square to the greater side and centers it, hiding both parts of the square that can't fit. So, for example, if the screen in 1200x800, then the point 0.0f,0.0f is mapped to 0,-200 (outside of the screen).

# Compiling and running

```bash
./build.sh
./build/SDL2Game
```
