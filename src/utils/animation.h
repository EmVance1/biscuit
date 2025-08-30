#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.h>
#include <stdint.h>
#include "atlas.h"


typedef struct sfuAnimatedSprite sfuAnimatedSprite;
#define self sfuAnimatedSprite


sfuAnimatedSprite* sfuAnimatedSprite_create(const sfuTextureAtlas* atlas);
void sfuAnimatedSprite_free(const self*);

void sfuAnimatedSprite_setAnimation(self*, const sfuTextureAtlas* atlas);
void sfuAnimatedSprite_setColor(    self*, sfColor color);
void sfuAnimatedSprite_setIsLooped( self*, bool loop);
void sfuAnimatedSprite_setIsPlaying(self*, bool playing);
void sfuAnimatedSprite_setFramerate(self*, float framerate);
void sfuAnimatedSprite_setRow(      self*, uint32_t row);
void sfuAnimatedSprite_setFrame(    self*, uint32_t frame);

const sfuTextureAtlas* sfuAnimatedSprite_getAnimation(const sfuAnimatedSprite* self);
sfColor  sfuAnimatedSprite_getColor(    const self*);
bool     sfuAnimatedSprite_getIsLooped( const self*);
bool     sfuAnimatedSprite_getIsPlaying(const self*);
float    sfuAnimatedSprite_getFramerate(const self*);
uint32_t sfuAnimatedSprite_getRow(      const self*);
uint32_t sfuAnimatedSprite_getFrame(    const self*);

sfFloatRect sfuAnimatedSprite_getLocalBounds( const self*);
sfFloatRect sfuAnimatedSprite_getGlobalBounds(const self*);

void sfuAnimatedSprite_update(self*, float dt);
void sfRenderWindow_drawAnimatedSprite(sfRenderWindow* window, const sfuAnimatedSprite* anim, const sfRenderStates* states);


#undef self

#endif
