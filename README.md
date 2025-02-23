# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- make enemies script better (choose side that they come from)
- in the Character class, theres code for firing projectiles, move that into a weapon class.
- implement collision attack (Character already has `attack` but it's missleading/ambiguous because the projectile is the actual attack of mainChar and the `attack` is the collision atack of the enemies)
- make enemies collide and push each other (dont let them stack on the screen)
- load a config json for characters (for specifying base attributes for size, velocity, texture, etc) so we can have many types.
- getMovementDirectionTowards has a limitation where diagonals get a different speed than vertical/horizontal because of the way it's calculated
- destroy projectiles when they are significantly off screen
- avoid rendering stuff off screen
- separate .h and .cpp files
- change update frequency shoudn't change how fast things change (use a ellapsed time factor)
- no texture is being deleted
- healthbar render is not taking into account the screen offset, possible resizing necessary, etc

# Compiling and running

```bash
./build.sh
./build/SDL2Game
```
