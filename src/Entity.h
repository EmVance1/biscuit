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
    sfVector2f lastDir;
    sfFloatRect rectBound;
    float speed;
    float acc;
    float dashSpeed;

    float health;
    float meleeRange;
    float meleeDamage;

    // ability cooldowns
    Cooldown dashCooldown;
    Cooldown attackCooldown;
    Cooldown attackAnim;
    
    float attackStartAngle;
} Entity;

void offsetEntity(Entity* entity, sfVector2f offset);

void moveEntity(Entity* entity);

void startDash(Entity* entity);

void addVelocity(Entity* entity, sfVector2f acceleration);

void setVelocity(Entity* entity, sfVector2f velocity);

void updateVelocity(Entity* entity);

void damage(Entity* entity, float damage);

void kill(Entity* entity);

Cooldown cooldownDefault();

void cooldownSet(Cooldown cd, float time);

void cooldownRestart(Cooldown cd);

// Returns remaining time on cooldown in seconds
float cooldownGet(Cooldown cd);

Cooldown cooldownCreate(float time);
#endif
