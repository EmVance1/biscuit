#include "world.h"
#include <stdlib.h>
#include <stdbool.h>


static bool* grid_createFromTilemap(const sfVector2u* indices, sfVector2u size);


World World_createFromIndices(const uint32_t* indices, const sfuTextureAtlas* atlas) {
    bool* grid = grid_createFromTilemap((sfVector2u*)indices, (sfVector2u){ 8, 8 });
    navMesh* navmesh = navMesh_createFromGrid((uint8_t*)grid, 8, 8, 1, 0, GEN_METHOD_FLOODFILL, 0.001f);
    return (World){
        .background = sfuTileMap_createFromIndices(atlas, indices, (sfVector2u){ 8, 8 }),
        .navmesh = navmesh,
        .colliders = navMesh_clonePolygons(navmesh),
    };
}

void World_free(const World* self) {
    sfuTileMap_free(self->background);
    navMesh_free(self->navmesh);
    navMesh_freePolygons(self->colliders);
}



static bool* grid_createFromTilemap(const sfVector2u* indices, sfVector2u size) {
    const static uint8_t grid_LUT[] = {
        0b0100, 0b1100, 0b1000, 0b1011, 0b1110, /* unused */ 0b0000, 0b0000, 0b0000,
        0b0110, 0b1111, 0b1001, 0b1101, 0b0111, /* unused */ 0b0000, 0b0000, 0b0000,
        0b0010, 0b0011, 0b0001, /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
    };
 
    bool* result = (bool*)calloc(sizeof(bool), (size.x+1) * (size.y+1));

    for (uint32_t y = 0; y < size.y; y++) {
        for (uint32_t x = 0; x < size.x; x++) {
            const sfVector2u idx = indices[y * size.y + x];
            const uint8_t bits = grid_LUT[idx.y * 8 + idx.x];
            if (bits & 1 << 0) {
                result[y * (size.y+1) + x] = true;
            }
            if (bits & 1 << 1) {
                result[y * (size.y+1) + x+1] = true;
            }
            if (bits & 1 << 2) {
                result[(y+1) * (size.y+1) + x+1] = true;
            }
            if (bits & 1 << 3) {
                result[(y+1) * (size.y+1) + x] = true;
            }
        }
    }

    return result;
}

