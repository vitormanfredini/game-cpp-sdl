cmake_minimum_required(VERSION 3.10)
project(SDL2Game)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")

enable_testing()
add_subdirectory(tests)
add_subdirectory(src)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(/usr/include/SDL2)
find_library(SDL2_IMAGE_LIBRARY SDL2_image REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE RESOURCE_FILES
    ${RESOURCE_DIR}/*.png
    ${RESOURCE_DIR}/*.jpg
    ${RESOURCE_DIR}/*.bmp
    ${RESOURCE_DIR}/*.ttf
    ${RESOURCE_DIR}/*.wav
)

set(RESOURCE_REGISTRY_HEADER "${CMAKE_BINARY_DIR}/resource_registry.h")
set(RESOURCE_NAMES "")
set(RESOURCE_VARS "")
set(RESOURCE_LENGTHS "")

foreach(RESOURCE ${RESOURCE_FILES})
    string(REPLACE "${RESOURCE_DIR}/" "" RESOURCE_REL_PATH ${RESOURCE})
    
    string(REPLACE "/" "_" HEADER_BASE_NAME "${RESOURCE_REL_PATH}")
    string(REPLACE "." "_" HEADER_BASE_NAME "${HEADER_BASE_NAME}")
    set(HEADER_NAME "${CMAKE_BINARY_DIR}/${HEADER_BASE_NAME}.h")
    set(BINARY_VAR_NAME "binary_${HEADER_BASE_NAME}")

    list(APPEND RESOURCE_NAMES "${RESOURCE_REL_PATH}")
    list(APPEND RESOURCE_VARS "${BINARY_VAR_NAME}")
    list(APPEND RESOURCE_LENGTHS "${BINARY_VAR_NAME}_len")

    add_custom_command(
        OUTPUT ${HEADER_NAME}
        COMMAND xxd -i ${RESOURCE} > ${HEADER_NAME}.tmp
        COMMAND sed "s/unsigned char .* =/unsigned char ${BINARY_VAR_NAME}[] =/g" ${HEADER_NAME}.tmp > ${HEADER_NAME}
        COMMAND sed -i '' "s/unsigned int .*_len =/unsigned int ${BINARY_VAR_NAME}_len =/g" ${HEADER_NAME}
        COMMAND rm -f ${HEADER_NAME}.tmp
        DEPENDS ${RESOURCE}
        COMMENT "Embedding ${RESOURCE} as ${HEADER_NAME}"
    )

    list(APPEND EMBEDDED_HEADERS ${HEADER_NAME})
endforeach()

string(REPLACE ";" "\",\"" RESOURCE_NAMES_STR "${RESOURCE_NAMES}")
string(REPLACE ";" "," RESOURCE_VARS_STR "${RESOURCE_VARS}")
string(REPLACE ";" "," RESOURCE_LENGTHS_STR "${RESOURCE_LENGTHS}")
list(LENGTH RESOURCE_NAMES RESOURCE_COUNT)

file(WRITE ${RESOURCE_REGISTRY_HEADER} "#pragma once\n\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "#include <string>\n\n")

foreach(HEADER ${EMBEDDED_HEADERS})
    get_filename_component(HEADER_NAME ${HEADER} NAME)
    file(APPEND ${RESOURCE_REGISTRY_HEADER} "#include \"${HEADER_NAME}\"\n")
endforeach()

file(APPEND ${RESOURCE_REGISTRY_HEADER} "\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "namespace ResourceRegistry {\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "    const int RESOURCE_COUNT = ${RESOURCE_COUNT};\n\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "    const char* RESOURCE_NAMES[] = {\"${RESOURCE_NAMES_STR}\"};\n\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "    unsigned char* RESOURCE_DATA[] = {${RESOURCE_VARS_STR}};\n\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "    unsigned int RESOURCE_LENGTHS[] = {${RESOURCE_LENGTHS_STR}};\n")
file(APPEND ${RESOURCE_REGISTRY_HEADER} "} // namespace ResourceRegistry\n")

add_custom_target(embed_resources ALL DEPENDS ${EMBEDDED_HEADERS} ${RESOURCE_REGISTRY_HEADER})

add_executable(SDL2Game ${SOURCE_FILES} ${EMBEDDED_HEADERS})
add_dependencies(SDL2Game embed_resources)
target_link_libraries(SDL2Game
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARY}
    PkgConfig::SDL2_TTF
)
target_include_directories(SDL2Game
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_BINARY_DIR}
)
