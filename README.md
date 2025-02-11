# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- fix bug where deleted enemies/projectiles are still being accessed (maybe use smart pointers)
- health bar needs to be rewritten (think of a way to render stuff that isn't just a texture, maybe separate the game rendering from the UI rendering)
- load a config json for characters (for specifying base attributes for size, velocity, texture, etc) so we can have many types.
- getMovementDirectionTowards has a limitation where diagonals get a different speed than vertical/horizontal because of the way it's calculated
- use unique pointers for enemies and projectiles (they currently are created using `new` and are not being destroyed)
- destroy projectlies when they are off screen
- avoid rendering off screen
