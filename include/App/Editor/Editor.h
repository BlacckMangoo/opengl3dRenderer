#pragma once
#include <cstdint>
// holds state for Gizmos , Selection etc.

struct Editor {
    // inspector state
    uint32_t selectedObjectIndex = 0 ;
    void SetSelectedObjectIndex(const uint32_t  index) {
        selectedObjectIndex = index;
    }

};
