#ifndef TILEMAP_H
#define TILEMAP_H
#include <SFML/Graphics.h>
#include <stdint.h>
#include "atlas.h"


typedef struct sfuQuad {
    sfVertex vertices[4];
} sfuQuad;

typedef struct sfuTileMap {
    const sfuTextureAtlas* atlas;
    sfTransformable* transform;
    sfVector2u dimensions;
    sfuQuad quads[];
} sfuTileMap;


sfuTileMap* sfuTileMap_createFromIndices(const sfuTextureAtlas* atlas, const uint32_t* indices, sfVector2u dimensions);
void sfuTileMap_free(const sfuTileMap* self);

void sfuTileMap_setTile(sfuTileMap* self, sfVector2u map_index, sfVector2u atlas_index);

void sfRenderWindow_drawTileMap(sfRenderWindow* self, const sfuTileMap* map, const sfRenderStates* states);


#endif
