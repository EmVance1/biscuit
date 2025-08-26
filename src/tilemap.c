#include "tilemap.h"
#include <SFML/Graphics.h>
#include <stdint.h>
#include <stdlib.h>
#include "atlas.h"

#include <stdio.h>


sfuTileMap* sfuTileMap_createFromIndices(const sfuTextureAtlas* atlas, const uint32_t* indices, sfVector2u dimensions) {
    const size_t count = dimensions.x * dimensions.y;
    sfuTileMap* map = (sfuTileMap*)malloc(sizeof(sfuTileMap) + count * sizeof(sfuQuad));
    if (!map) { return NULL; }

    map->atlas = atlas;
    map->transform = sfTransformable_create();
    map->dimensions = dimensions;
    for (size_t i = 0; i < count; i++) {
        const sfVector2u index = (sfVector2u){ (uint32_t)i % dimensions.x, (uint32_t)i / dimensions.x };
        const sfVector2u position = sfVec2u_compMul(index, atlas->cellsize);
        map->quads[i].vertices[0].position = (sfVector2f){ (float)position.x, (float)position.y };
        map->quads[i].vertices[1].position = (sfVector2f){ (float)position.x + (float)atlas->cellsize.x, (float)position.y };
        map->quads[i].vertices[2].position = (sfVector2f){ (float)position.x + (float)atlas->cellsize.x, (float)position.y + (float)atlas->cellsize.y };
        map->quads[i].vertices[3].position = (sfVector2f){ (float)position.x, (float)position.y + (float)atlas->cellsize.y };

        const sfIntRect texture = sfuTextureAtlas_getTextureRect(atlas, (sfVector2u){ indices[i * 2], indices[i * 2 + 1] });
        map->quads[i].vertices[0].texCoords = (sfVector2f){ (float)texture.left, (float)texture.top };
        map->quads[i].vertices[1].texCoords = (sfVector2f){ (float)texture.left + (float)texture.width, (float)texture.top };
        map->quads[i].vertices[2].texCoords = (sfVector2f){ (float)texture.left + (float)texture.width, (float)texture.top + (float)texture.height };
        map->quads[i].vertices[3].texCoords = (sfVector2f){ (float)texture.left, (float)texture.top + (float)texture.height };

        map->quads[i].vertices[0].color = (sfColor){ .r=255, .g=255, .b=255, .a=255 };
        map->quads[i].vertices[1].color = (sfColor){ .r=255, .g=255, .b=255, .a=255 };
        map->quads[i].vertices[2].color = (sfColor){ .r=255, .g=255, .b=255, .a=255 };
        map->quads[i].vertices[3].color = (sfColor){ .r=255, .g=255, .b=255, .a=255 };
    }

    return map;
}

void sfuTileMap_free(const sfuTileMap* self) {
    sfTransformable_destroy(self->transform);
    free((sfuTextureAtlas*)self);
}


void sfuTileMap_setTile(sfuTileMap* self, sfVector2u map_index, sfVector2u atlas_index) {
    const size_t index = map_index.x + map_index.y * self->dimensions.x;
    const sfIntRect texture = sfuTextureAtlas_getTextureRect(self->atlas, atlas_index);
    self->quads[index].vertices[0].texCoords = (sfVector2f){ (float)texture.left, (float)texture.top };
    self->quads[index].vertices[1].texCoords = (sfVector2f){ (float)texture.left + (float)texture.width, (float)texture.top };
    self->quads[index].vertices[2].texCoords = (sfVector2f){ (float)texture.left + (float)texture.width, (float)texture.top + (float)texture.height };
    self->quads[index].vertices[3].texCoords = (sfVector2f){ (float)texture.left, (float)texture.top + (float)texture.height };
}


void sfRenderWindow_drawTileMap(sfRenderWindow* self, const sfuTileMap* map, const sfRenderStates* states) {
    sfRenderStates mystates = sfRenderStates_default();
    mystates.texture = map->atlas->texture;
    mystates.transform = sfTransformable_getTransform(map->transform);
    if (states) {
        sfTransform_combine(&mystates.transform, &states->transform);
    }
    sfRenderWindow_drawPrimitives(self, (sfVertex*)map->quads, 4 * map->dimensions.x * map->dimensions.y, sfQuads, &mystates);
}

