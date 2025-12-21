#pragma once
#include <cstdint>

// GENERATE ID FOR GAMEOBJECTS
inline uint32_t GenerateGameObjectID() {
    static uint32_t currentID = 0;
    return currentID++;
}

