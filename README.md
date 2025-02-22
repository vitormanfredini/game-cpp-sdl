# SDL Game

This is a project I'm doing for studying C++ and SDL.

TODO:
- load a config json for characters (for specifying base attributes for size, velocity, texture, etc) so we can have many types.
- getMovementDirectionTowards has a limitation where diagonals get a different speed than vertical/horizontal because of the way it's calculated
- destroy projectiles when they are significantly off screen
- avoid rendering stuff off screen
- separate .h and .cpp files
- mexer no updatesPerSecond não deve ter efeito no tempo que as coisas andam na tela
- nenhuma texture está sendo destruída/deletada. resolver.
- healthbar render is not taking into account the screen offset, possible resizing necessary, etc
