#pragma once

#include <memory>
#include "GameObject/Character/CharacterType.h"
#include "SpawnSchedule.h"

class SpawnScheduleFactory {
public:

    std::unique_ptr<SpawnSchedule> makeStage1(){
        std::unique_ptr<SpawnSchedule> stage = std::make_unique<SpawnSchedule>();

        for(int c=0;c<100;c++){
            int framesOffset = c * 6800;
            stage->addKeyframe({ framesOffset + 240, 1 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 540, 3 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 650, 1 * (c+1), CharacterType::Boss });
            stage->addKeyframe({ framesOffset + 1000, 3 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 1050, 2 * (c+1), CharacterType::Bigger });
            stage->addKeyframe({ framesOffset + 1500, 5 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 1350, 2 * (c+1), CharacterType::Bigger });

            stage->addKeyframe({ framesOffset + 2300, 5 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 2400, 3 * (c+1), CharacterType::Bigger });
            stage->addKeyframe({ framesOffset + 2500, 2 * (c+1), CharacterType::Regular });

            stage->addKeyframe({ framesOffset + 3500, 5 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 3600, 3 * (c+1), CharacterType::Bigger });
            stage->addKeyframe({ framesOffset + 3700, 5 * (c+1), CharacterType::Regular });

            stage->addKeyframe({ framesOffset + 4150, 1 * (c+1), CharacterType::Boss });

            stage->addKeyframe({ framesOffset + 4600, 22 * (c+1), CharacterType::Regular });

            stage->addKeyframe({ framesOffset + 5100, 16 * (c+1), CharacterType::Bigger });

            stage->addKeyframe({ framesOffset + 5100, 1 * (c+1), CharacterType::Boss });

            stage->addKeyframe({ framesOffset + 5500, 15 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 5500, 20 * (c+1), CharacterType::Bigger });

            stage->addKeyframe({ framesOffset + 6000, 3 * (c+1), CharacterType::Boss });

            stage->addKeyframe({ framesOffset + 6700, 10 * (c+1), CharacterType::Regular });
            stage->addKeyframe({ framesOffset + 6700, 10 * (c+1), CharacterType::Bigger });
            stage->addKeyframe({ framesOffset + 6700, 10 * (c+1), CharacterType::Boss });
        }

        return std::move(stage);
    }

};
