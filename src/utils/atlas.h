#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H
#include <SFML/Graphics.h>
#include <stdbool.h>


typedef struct sfuTextureAtlas {
    sfVector2u dimensions;
    sfVector2u cellsize;
    sfTexture* texture;
} sfuTextureAtlas;


sfuTextureAtlas* sfuTextureAtlas_createFromImage(const sfImage* image, sfVector2u dimensions);
sfuTextureAtlas* sfuTextureAtlas_createFromFile(const char* filename, sfVector2u dimensions);
void sfuTextureAtlas_free(const sfuTextureAtlas* self);

void sfuTextureAtlas_setSmooth(sfuTextureAtlas* self, bool smooth);
bool sfuTextureAtlas_isSmooth(const sfuTextureAtlas* self);

sfIntRect sfuTextureAtlas_getTextureRect(const sfuTextureAtlas* self, sfVector2u indices);


#endif
