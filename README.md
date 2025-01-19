# SDL Game

This is a project I'm doing for studying C++ and SDL.

Todo:
- render stuff proportionally to the width of the window.
- allow character to move outside of the screen (screen should follow the main character)
- create a Engine class that will take care of game logic, loading assets and destroying them.
- getMovementDirectionTowards has a limitation where diagonals get a different speed than vertical/horizontal because of the way it's calculated
- cache textures in `loadTexture` so BinaryResourceLoader::toTexture doesnt need to create one everytime it's called with the same filename
- make Renderer do the SDL render instead of the object itself
- use unique pointers for enemies and projectiles (they currently are created using `new` and are not being destroyed)
