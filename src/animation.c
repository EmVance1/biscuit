#include "animation.h"
#include "SFML/Graphics/RenderWindow.h"
#include <stdlib.h>


typedef struct sfuAnimatedSprite {
    const sfuTextureAtlas* atlas;
    sfTransformable* transform;
    sfVertex   vertices[4];
    sfVector2u index;
    sfIntRect  textureRect;

    float frametime;
    float time;
    bool  playing;
    bool  looping;
} sfuAnimatedSprite;


static void sfuAnimatedSprite_updateFrame(sfuAnimatedSprite* self) {
    self->textureRect = sfuTextureAtlas_getTextureRect(self->atlas, self->index);
    self->vertices[0].texCoords =
        (sfVector2f){ (float)self->textureRect.left, (float)self->textureRect.top };
    self->vertices[1].texCoords =
        (sfVector2f){ (float)(self->textureRect.left + self->textureRect.width), (float)self->textureRect.top };
    self->vertices[2].texCoords =
        (sfVector2f){ (float)self->textureRect.left, (float)(self->textureRect.top + self->textureRect.height) };
    self->vertices[3].texCoords =
        (sfVector2f){ (float)(self->textureRect.left + self->textureRect.width), (float)(self->textureRect.top + self->textureRect.width) };
}


sfuAnimatedSprite* sfuAnimatedSprite_create(const sfuTextureAtlas* atlas) {
    sfuAnimatedSprite* self = (sfuAnimatedSprite*)malloc(sizeof(sfuAnimatedSprite));
    self->transform = sfTransformable_create();
    self->frametime = 0.25f;
    self->time = 0.f;
    self->playing = false;
    self->looping = false;
    sfuAnimatedSprite_setAnimation(self, atlas);
    return self;
}

void sfuAnimatedSprite_free(const sfuAnimatedSprite* self) {
    sfTransformable_destroy(self->transform);
    free((sfuAnimatedSprite*)self);
}


void sfuAnimatedSprite_setAnimation(sfuAnimatedSprite* self, const sfuTextureAtlas* atlas) {
    self->atlas = atlas;
    self->index = (sfVector2u){ 0, 0 };
    self->textureRect = sfuTextureAtlas_getTextureRect(atlas, (sfVector2u){ 0, 0 });
    self->vertices[0].position = (sfVector2f){ 0.f, 0.f };
    self->vertices[1].position = (sfVector2f){ (float)self->textureRect.width, 0.f };
    self->vertices[2].position = (sfVector2f){ 0.f, (float)self->textureRect.height };
    self->vertices[3].position = (sfVector2f){ (float)self->textureRect.width, (float)self->textureRect.height };
    sfuAnimatedSprite_updateFrame(self);
}

void sfuAnimatedSprite_setColor(sfuAnimatedSprite* self, sfColor color) {
    for (int i = 0; i < 4; i++) {
        self->vertices[i].color = color;
    }
}

void sfuAnimatedSprite_setIsLooped(sfuAnimatedSprite* self, bool loop) {
    self->looping = loop;
}

void sfuAnimatedSprite_setIsPlaying(sfuAnimatedSprite* self, bool playing) {
    self->playing = playing;
}

void sfuAnimatedSprite_setFramerate(sfuAnimatedSprite* self, float framerate) {
    self->frametime = 1.f / framerate;
}

void sfuAnimatedSprite_setRow(sfuAnimatedSprite* self, uint32_t row) {
    self->index.y = row;
    sfuAnimatedSprite_updateFrame(self);
}

void sfuAnimatedSprite_setFrame(sfuAnimatedSprite* self, uint32_t frame) {
    self->index.x = frame;
    sfuAnimatedSprite_updateFrame(self);
}


const sfuTextureAtlas* sfuAnimatedSprite_getAnimation(const sfuAnimatedSprite* self) {
    return self->atlas;
}

sfColor sfuAnimatedSprite_getColor(const sfuAnimatedSprite* self) {
    return self->vertices[0].color;
}

bool sfuAnimatedSprite_getIsLooped(const sfuAnimatedSprite* self) {
    return self->looping;
}

bool sfuAnimatedSprite_getIsPlaying(const sfuAnimatedSprite* self) {
    return self->playing;
}

float sfuAnimatedSprite_getFramerate(const sfuAnimatedSprite* self) {
    return 1.f / self->frametime;
}

uint32_t sfuAnimatedSprite_getRow(const sfuAnimatedSprite* self) {
    return self->index.y;
}

uint32_t sfuAnimatedSprite_getFrame(const sfuAnimatedSprite* self) {
    return self->index.x;
}


sfFloatRect sfuAnimatedSprite_getLocalBounds(const sfuAnimatedSprite* self) {
    return (sfFloatRect){ self->vertices[0].position.x, self->vertices[0].position.y,
        self->vertices[2].position.x - self->vertices[0].position.x, self->vertices[2].position.y - self->vertices[0].position.y };
}

sfFloatRect sfuAnimatedSprite_getGlobalBounds(const sfuAnimatedSprite* self) {
    sfTransform transform = sfTransformable_getTransform(self->transform);
    return sfTransform_transformRect(&transform, sfuAnimatedSprite_getLocalBounds(self));
}


void sfuAnimatedSprite_update(sfuAnimatedSprite* self, float dt) {
    if (self->playing) {
        self->time += dt;
        if (self->time >= self->frametime) {
            self->time -= self->frametime;
            const uint32_t max_x = self->atlas->dimensions.x;
            if (++self->index.x == max_x) {
                if (self->looping) {
                    self->index.x %= max_x;
                } else {
                    self->index.x = max_x - 1;
                }
            }
            sfuAnimatedSprite_updateFrame(self);
        }
    }
}


void sfRenderWindow_drawAnimatedSprite(sfRenderWindow* window, const sfuAnimatedSprite* self, const sfRenderStates* states) {
    sfRenderStates mystates = sfRenderStates_default();
    mystates.texture = self->atlas->texture;
    mystates.transform = sfTransformable_getTransform(self->transform);
    if (states) {
        sfTransform_combine(&mystates.transform, &states->transform);
    }
    sfRenderWindow_drawPrimitives(window, self->vertices, 4, sfTriangleStrip, &mystates);
}

