#include "world.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


static bool* grid_createFromTilemap(const sfVector2u* indices, sfVector2u size);


World World_createFromIndices(const uint32_t* indices, sfVector2u size, const sfuTextureAtlas* atlas) {
    bool* grid = grid_createFromTilemap((sfVector2u*)indices, size);

    /*
    for (uint32_t y = 0; y < size.y+1; y++) {
        for (uint32_t x = 0; x < size.x+1; x++) {
            printf("%d", (int)grid[y * (size.x+1) + x]);
        }
        printf("\n");
    }
    */

    sfuTileMap* background = sfuTileMap_createFromIndices(atlas, indices, size);
    sfTransformable_move(background->transform, (sfVector2f){ (float)atlas->cellsize.x * 0.5f, (float)atlas->cellsize.y * 0.5f });
    navMesh* navmesh = navMesh_createFromGrid((uint8_t*)grid, size.x+1, size.y+1, 1, 0, GEN_METHOD_FLOODFILL, 0.001f);
    navPolygonArray* colliders = navMesh_clonePolygons(navmesh);
    sfFloatRect door0 = (sfFloatRect) {size.x*atlas->cellsize.x,size.y/2.f*atlas->cellsize.x-30,15,25};
    sfFloatRect door1 = (sfFloatRect) {size.x*atlas->cellsize.x,size.y/2.f*atlas->cellsize.x+30,15,25};

    return (World){
        .background = background,
        .navmesh    = navmesh,
        .colliders  = colliders,
        .mesh_to_world = (float)atlas->cellsize.x,
        .world_to_mesh = 1.f / (float)atlas->cellsize.x,
        .gridsize = (sfVector2f){ (float)size.x+1.f, (float)size.y+1.f },
        .doors = {door0, door1},
    };
}

void World_free(const World* self) {
    sfuTileMap_free(self->background);
    navMesh_free(self->navmesh);
    navMesh_freePolygons(self->colliders);
}



static bool* grid_createFromTilemap(const sfVector2u* indices, sfVector2u size) {
    const static uint8_t grid_LUT[] = {
        0b0100, 0b1100, 0b1000, 0b1011, 0b0111, /* unused */ 0b0000, 0b0000, 0b0000,
        0b0110, 0b1111, 0b1001, 0b1101, 0b1110, /* unused */ 0b0000, 0b0000, 0b0000,
        0b0010, 0b0011, 0b0001, /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
        /* unused */ 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000, 0b0000,
    };
 
    bool* result = (bool*)calloc((size.x+1) * (size.y+1), sizeof(bool));

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

