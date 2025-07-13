#pragma once

#include <unordered_map>

template <typename Resource>
class CacheManager {

public:
    using ConstructorCallback = std::function<Resource(std::string&)>;
    using DestructorCallback = std::function<void(Resource)>;

    CacheManager(
        ConstructorCallback constructorCallback = [](std::string){},
        DestructorCallback destructorCallback = [](Resource){}
    )
        :   constructorCallback(constructorCallback),
            destructorCallback(destructorCallback) {}

    ~CacheManager(){
        clear();
    }

    bool exists(std::string identifier){
        auto it = stringIdToResource.find(identifier);
        return it != stringIdToResource.end();
    }

    bool exists(int id){
        auto it = intIdToResource.find(id);
        return it != intIdToResource.end();
    }

    int load(std::string identifier){

        auto it = stringIdToIntId.find(identifier);
        if (it != stringIdToIntId.end()) {
            return stringIdToIntId[identifier];
        }

        Resource resource = constructorCallback(identifier);

        stringIdToResource[identifier] = resource;
        intIdToResource[nextId] = resource;
        stringIdToIntId[identifier] = nextId;
        nextId++;

        return nextId - 1;
    }

    Resource get(int id){
        auto it = intIdToResource.find(id);
        if (it == intIdToResource.end()) {
            std::cerr << "CacheManager get(): Error: Resource (" << id << ") not found." << std::endl;
        }
        return intIdToResource[id];
    }

    void remove(std::string identifier){
        auto it = stringIdToResource.find(identifier);
        if (it == stringIdToResource.end()) {
            return;
        }
        destructorCallback(it->second);
        stringIdToResource.erase(it);
    }

    void clear() {
        for (auto& pair : stringIdToResource) {
            destructorCallback(pair.second);
        }
        stringIdToResource.clear();
        intIdToResource.clear();
        stringIdToIntId.clear();
    }

private:

    std::unordered_map<std::string, Resource> cacheStore;

    std::unordered_map<std::string, Resource> stringIdToResource;
    std::unordered_map<int, Resource> intIdToResource;
    std::unordered_map<std::string, int> stringIdToIntId;

    int nextId = 1;

    ConstructorCallback constructorCallback;
    DestructorCallback destructorCallback;

};
