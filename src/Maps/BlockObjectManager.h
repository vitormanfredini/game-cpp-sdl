#pragma once

#include <vector>
#include "GameObject/GameObject.h"
#include <unordered_map>
#include <string>
#include <memory>

class BlockObjectManager {

public:

    void update(float cameraPosX, float cameraPosY){
        int truncatedX = static_cast<int>(cameraPosX);
        int truncatedY = static_cast<int>(cameraPosY);
        
        if(truncatedX == cacheBlockX && truncatedY == cacheBlockY){
            return;
        }

        nearObjectsCache.clear();

        for(int x=truncatedX-1; x <= truncatedX + 1; x++){
            for(int y=truncatedY-1; y <= truncatedY + 1; y++){
                std::string key = createKey(x,y);
                auto it = objectBlockMap.find(key);
                if (it != objectBlockMap.end()) {
                    for(std::unique_ptr<GameObject>& item : it->second){
                        nearObjectsCache.push_back(item.get());
                    }
                }
            }
        }

        cacheBlockX = truncatedX;
        cacheBlockY = truncatedY;
    }

    void addItem(int blockX, int blockY, std::unique_ptr<GameObject> item, bool alsoAddToCache = false){
        if(alsoAddToCache){
            nearObjectsCache.push_back(item.get());
        }
        std::string key = createKey(blockX, blockY);
        objectBlockMap[key].push_back(std::move(item));
    }

    std::vector<GameObject*>& getNearObjects(){
        return nearObjectsCache;
    }

private:

    std::string createKey(int x, int y) {
        return std::to_string(x) + "_" + std::to_string(y);
    }

    std::unordered_map<std::string, std::vector<std::unique_ptr<GameObject>>> objectBlockMap;

    std::vector<GameObject*> nearObjectsCache;
    int cacheBlockX = -9999;
    int cacheBlockY = 9999;

};
