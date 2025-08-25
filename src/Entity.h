#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics.h"

typedef struct Entity {
    sfVector2f position;
    sfFloatRect rectBound;
} Entity;

void moveEntity(Entity* entity, sfVector2f offset);

#endif
