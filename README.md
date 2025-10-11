# SDL Game

This is a game!!!!!!!1

TODO:
- draw different types of debris (maybe specify what type of debris type in each enemy, so each type can drop a different type when they die)
- make debris animate when enemies die
- make enemies do some animation when they die before disappearing
- add tiles dynamically (instead of loading all of them at once at the begining)
- Make upgrade menu be controlled by the keyboard also
- Add controller support
- AudioEngine Callback: Lock is too long. There's an opportunity to lock for less time: Lock -> copy vector of soundsPlaying to localSoundsPlaying, update positions of soundsPlaying -> remove finished -> Unlock -> Do the mixing using localSoundsPlaying.
- Make a way to specify volume for each sound in the mix.
- reorder renderables (enemies, items and maybe projectiles) from bottom to top to make sure they don't render in front of others that are in the the front.
- GameEngine: make a "startMission" where everything is loaded into place before the mission begins instead of organizing everything inside the main.cpp.
- use asserts instead of cerr. #include <cassert> assert(b != 0 && "Division by zero");
- WeaponUpgrade needs to specify which weapon (it's currently applying to all weapons in the vector)
- make a weapon factory with prototype pattern
- MenuFactory edge case: when no upgrades are available. (implement regular items to replace the upgrades)
- MenuFactory: implement a way to calculate buttons sizes and positions base an area for them to fill.
- destroy projectiles when they are significantly off screen
- avoid rendering stuff off screen
- update to SDL 3
- separate .h and .cpp files
- no texture is being unloaded. implement a way of telling the TextureManager the texture's context (menu, game world, in-game UI, etc) so we can unload them without unloading textures that are being used.
- bug: sometimes it doesn't find the closest enemy and fires at an enemy far away
- FUTURE: keep renderables from last frame in a buffer, if rendering is faster than updates, use last and current renderables to predict where everything will be. (that allows for higher framerates without changing the update rate)
- add icon

# Rendering specifics

The renderer accepts virtual values and maps them onto the screen. If the screen is a perfect square (for example, 500x500) then 0.0f,0.0f is exactly on the top left corner and 1.0fx1.0f is exactly on the bottom right corner.
But when the screen isn't a perfect square, it scales the virtual square to the greater side and centers it, hiding both parts of the square that can't fit. So, for example, if the screen in 1200x800, then the point 0.0f,0.0f is mapped to 0,-200 (outside of the screen).

# Compiling and running

```bash
sudo apt-get install libsdl2-mixer-dev
```

```bash
./build.sh
./build/SDL2Game
```
