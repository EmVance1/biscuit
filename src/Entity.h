#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.h>


typedef struct Cooldown {
    sfClock* clock;
    float cooldownLength;
} Cooldown;

typedef struct Entity {
    sfVector2f position;
    sfVector2f velocity;
    sfFloatRect rectBound;
    float speed;
    float acc;
    float dashSpeed;

    Cooldown dashCooldown;
} Entity;

void Entity_move(Entity* entity, sfVector2f offset);

void Entity_startDash(Entity* entity);

void Entity_addVelocity(Entity* entity, sfVector2f acceleration);

void Entity_setVelocity(Entity* entity, sfVector2f velocity);

void Entity_updateVelocity(Entity* entity);


#endif
