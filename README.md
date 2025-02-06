# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- reposition ground tiles as the character moves
- set up tests
- health bar needs to be rewritten (think of a way to render stuff that isn't just a texture, maybe separate the game rendering from the UI rendering)
- create a Engine class that will take care of game logic, loading assets and destroying them.
- getMovementDirectionTowards has a limitation where diagonals get a different speed than vertical/horizontal because of the way it's calculated
- cache textures in `loadTexture` so BinaryResourceLoader::toTexture doesnt need to create one everytime it's called with the same filename (and destroy them when game finishes)
- use unique pointers for enemies and projectiles (they currently are created using `new` and are not being destroyed)
- destroy projectlies when they are off screen
- avoid rendering off screen
