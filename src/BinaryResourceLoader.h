#pragma once

#include "resource_registry.h"
#include <string.h>
#include <iostream>

struct BinaryResource {
    unsigned char* data;
    unsigned int length;

    BinaryResource(unsigned char* data, unsigned int length) : data(data), length(length) {}
    BinaryResource() : data(nullptr), length(0) {}
};

class BinaryResourceLoader {

public:

    static BinaryResource getBinaryResource(const char* filename){
        for (int i = 0; i < ResourceRegistry::RESOURCE_COUNT; i++) {
            if (strcmp(ResourceRegistry::RESOURCE_NAMES[i], filename) == 0) {
                return BinaryResource(
                    ResourceRegistry::RESOURCE_DATA[i],
                    ResourceRegistry::RESOURCE_LENGTHS[i]
                );
                break;
            }
        }
        std::cerr << "Binary file " << filename << " could not be found." << std::endl;
        return BinaryResource();
    }

};
