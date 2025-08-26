#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/Graphics.h"

typedef struct Cooldown {
    sfClock* clock;
    float cooldownLength;
} Cooldown;

typedef struct Entity {
    sfVector2f position;
    sfVector2f velocity;
    sfFloatRect rectBound;
    float speed;
    float dashSpeed;

    Cooldown dashCooldown;
} Entity;

void moveEntity(Entity* entity, sfVector2f offset);

void startDash(Entity* entity);

void addVelocity(Entity* entity, sfVector2f velocity);

void setVelocity(Entity* entity, sfVector2f velocity);

void updateVelocity(Entity* entity);
#endif
