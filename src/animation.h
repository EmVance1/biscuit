#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.h>
#include <stdint.h>
#include "atlas.h"


typedef struct sfuAnimatedSprite sfuAnimatedSprite;


sfuAnimatedSprite* sfuAnimatedSprite_create(const sfuTextureAtlas* atlas);
void sfuAnimatedSprite_free(const sfuAnimatedSprite* self);

void sfuAnimatedSprite_setAnimation(sfuAnimatedSprite* self, const sfuTextureAtlas* atlas);
void sfuAnimatedSprite_setColor(    sfuAnimatedSprite* self, sfColor color);
void sfuAnimatedSprite_setIsLooped( sfuAnimatedSprite* self, bool loop);
void sfuAnimatedSprite_setIsPlaying(sfuAnimatedSprite* self, bool playing);
void sfuAnimatedSprite_setFramerate(sfuAnimatedSprite* self, float framerate);
void sfuAnimatedSprite_setRow(      sfuAnimatedSprite* self, uint32_t row);
void sfuAnimatedSprite_setFrame(    sfuAnimatedSprite* self, uint32_t frame);

const sfuTextureAtlas* sfuAnimatedSprite_getAnimation(const sfuAnimatedSprite* self);
sfColor  sfuAnimatedSprite_getColor(    const sfuAnimatedSprite* self);
bool     sfuAnimatedSprite_getIsLooped( const sfuAnimatedSprite* self);
bool     sfuAnimatedSprite_getIsPlaying(const sfuAnimatedSprite* self);
float    sfuAnimatedSprite_getFramerate(const sfuAnimatedSprite* self);
uint32_t sfuAnimatedSprite_getRow(      const sfuAnimatedSprite* self);
uint32_t sfuAnimatedSprite_getFrame(    const sfuAnimatedSprite* self);

sfFloatRect sfuAnimatedSprite_getLocalBounds( const sfuAnimatedSprite* self);
sfFloatRect sfuAnimatedSprite_getGlobalBounds(const sfuAnimatedSprite* self);

void sfuAnimatedSprite_update(sfuAnimatedSprite* self, float dt);
void sfRenderWindow_drawAnimatedSprite(sfRenderWindow* window, const sfuAnimatedSprite* self, const sfRenderStates* states);


#endif
