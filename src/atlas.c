#include "atlas.h"
#include <stdlib.h>


sfuTextureAtlas* sfuTextureAtlas_createFromImage(const sfImage* image, sfVector2u dimensions) {
    sfuTextureAtlas* atlas = (sfuTextureAtlas*)malloc(sizeof(sfuTextureAtlas));
    atlas->dimensions = dimensions,
    atlas->texture = sfTexture_createFromImage(image, NULL);
    const sfVector2u size = sfTexture_getSize(atlas->texture);
    atlas->cellsize = sfVec2u_compDivFloor(size, dimensions);
    return atlas;
}

sfuTextureAtlas* sfuTextureAtlas_createFromFile(const char* filename, sfVector2u dimensions) {
    sfuTextureAtlas* atlas = (sfuTextureAtlas*)malloc(sizeof(sfuTextureAtlas));
    atlas->dimensions = dimensions,
    atlas->texture = sfTexture_createFromFile(filename, NULL);
    const sfVector2u size = sfTexture_getSize(atlas->texture);
    atlas->cellsize = sfVec2u_compDivFloor(size, dimensions);
    return atlas;
}

void sfuTextureAtlas_free(const sfuTextureAtlas* self) {
    sfTexture_destroy(self->texture);
    free((sfuTextureAtlas*)self);
}


void sfuTextureAtlas_setSmooth(sfuTextureAtlas* self, bool smooth) {
    sfTexture_setSmooth(self->texture, smooth);
}

bool sfuTextureAtlas_isSmooth(const sfuTextureAtlas* self) {
    return sfTexture_isSmooth(self->texture);
}


sfIntRect sfuTextureAtlas_getTextureRect(const sfuTextureAtlas* self, sfVector2u indices) {
    const sfVector2u temp = sfVec2u_compMul(self->cellsize, indices);
    return (sfIntRect){ (int)temp.x, (int)temp.y, self->cellsize.x, self->cellsize.y };
}

