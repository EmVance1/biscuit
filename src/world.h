#ifndef WORLD_H
#define WORLD_H
#include "tilemap.h"
#include <navmesh/c/lib.h>


typedef struct World {
    sfuTileMap* background;
    navMesh* navmesh;
    navPolygonArray* colliders;
} World;


World World_createFromIndices(const uint32_t* indices, sfVector2u size, const sfuTextureAtlas* atlas);

void World_free(const World* self);


#endif
