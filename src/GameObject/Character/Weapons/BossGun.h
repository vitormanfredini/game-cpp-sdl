#pragma once

#include <SDL2/SDL_render.h>
#include <memory>
#include <unordered_map>
#include "WeaponComponent.h"
#include "RandomGenerator.h"
#include "Projectile.h"
#include "Utils.h"


class BossGun: public WeaponComponent {

public:
    BossGun() {
        firingModes[FiringMode::Idle] = {120, 9999};
        firingModes[FiringMode::Rotating] = {240, 5};
        firingModes[FiringMode::Waves] = {(90*8) + 1, 90};

        nextFiringMode();
    }

    void setProjectileTexture(SDL_Texture* newTexture){
        texture = newTexture;
    }

    std::unique_ptr<WeaponComponent> clone() const override {
        return std::make_unique<BossGun>(*this);
    }

private:

    SDL_Texture* texture;

    enum class FiringMode {
        Rotating,
        Waves,
        Idle
    };

    struct FiringModeData {
        int durationUpdates;
        int updatesBetweenFires;

        FiringModeData(int durationUpdates = 180, int updatesBetweenFires = 10)
            :
            durationUpdates(durationUpdates),
            updatesBetweenFires(updatesBetweenFires) {
                //
            }
    };

    std::vector<FiringMode> firingModeCycle = {
        FiringMode::Idle,
        FiringMode::Waves,
        FiringMode::Idle,
        FiringMode::Rotating
    };
    size_t currentFiringModeCycleIndex = 0;

    double firingRotationAngle = 0;

    bool firingWavesIsVertical = true;

    FiringMode firingMode = FiringMode::Idle;

    std::unordered_map<FiringMode, FiringModeData> firingModes;

    int currentFiringModeUpdates;
    int updatesBetweenFires;
    int updatesUntilNextMode;
    FiringMode currentFiringMode;

    void update(){
        currentFiringModeUpdates += 1;
        if(currentFiringModeUpdates >= updatesBetweenFires){
            currentFiringModeUpdates = 0;
        }

        updatesUntilNextMode -= 1;
        if(updatesUntilNextMode <= 0){
            nextFiringMode();
        }
    }

    void nextFiringMode(){
        currentFiringModeCycleIndex++;
        if(currentFiringModeCycleIndex >= firingModeCycle.size()){
            currentFiringModeCycleIndex = 0;
        }

        currentFiringMode = firingModeCycle[currentFiringModeCycleIndex];
        updatesBetweenFires = firingModes[currentFiringMode].updatesBetweenFires;
        updatesUntilNextMode = firingModes[currentFiringMode].durationUpdates;
        currentFiringModeUpdates = 0;
    }

    std::vector<std::unique_ptr<Projectile>> fire(Character* originChar){
        std::vector<std::unique_ptr<Projectile>> newProjectiles = {};

        if(currentFiringModeUpdates == (updatesBetweenFires - 1)){

            if(currentFiringMode == FiringMode::Rotating){
                firingRotationAngle += 5.0d;
                if(firingRotationAngle > 360.0d){
                    firingRotationAngle = firingRotationAngle - 360.0d;
                }

                std::unique_ptr<Projectile> newProjectile = createProjectile(originChar);
                newProjectile->setAttack(attack);
                newProjectile->setSound(fireSound);
                newProjectile->setDirection(MovementDirection(1.0f,0.0f).rotated(Utils::degreesToRadians(firingRotationAngle)));
                newProjectile->setVelocity(0.005f);
                newProjectiles.push_back(std::move(newProjectile));
            }

            if(currentFiringMode == FiringMode::Waves){
                firingWavesIsVertical = !firingWavesIsVertical;

                double angleRange = 360.0d / 4.0d;
                size_t rangeProjectiles = 10;
                double anglePerProjectile = angleRange / static_cast<double>(rangeProjectiles - 1);

                std::vector<double> angles = {};

                for(size_t c=0; c<rangeProjectiles; c++){
                    angles.push_back( (firingWavesIsVertical ? -45.0d : 45.0d ) + (static_cast<double>(c)*anglePerProjectile) );
                }
                for(size_t c=0; c<rangeProjectiles; c++){
                    angles.push_back( (firingWavesIsVertical ? 135.0d : 225.0d ) + (static_cast<double>(c)*anglePerProjectile) );
                }

                for(double angle : angles){
                    std::unique_ptr<Projectile> newProjectile = createProjectile(originChar);
                    newProjectile->setAttack(attack);
                    newProjectile->setSound(fireSound);
                    newProjectile->setDirection(MovementDirection(1.0f,0.0f).rotated(Utils::degreesToRadians(angle)));
                    newProjectile->setVelocity(0.005f);
                    newProjectiles.push_back(std::move(newProjectile));
                }
            }

        }

        return newProjectiles;
    }

    std::unique_ptr<Projectile> createProjectile(Character* originChar){
        std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
        projectile->setAttack(attack);
        projectile->setPosition(
            originChar->getX(),
            originChar->getY()
        );
        projectile->setZ(originChar->getHeight() / 2);
        projectile->setSize(0.03,0.03);
        projectile->addRenderComponent(std::make_unique<SpriteRenderer>(
            texture,
            Alignment::Centered
        ));
        projectile->setCollisionComponent(std::make_unique<BoxCollider>(
            0.03,
            0.03,
            Alignment::Centered
        ));
        projectile->setDurationInUpdates(240);
        return projectile;
    }

};