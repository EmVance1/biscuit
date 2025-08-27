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

void Entity_offset(Entity* entity, sfVector2f offset);

void Entity_move(Entity* entity);

void Entity_startDash(Entity* entity);

void Entity_addVelocity(Entity* entity, sfVector2f acceleration);

void Entity_setVelocity(Entity* entity, sfVector2f velocity);

void Entity_updateVelocity(Entity* entity);

void Entity_damage(Entity* entity, float damage);

void Entity_kill(Entity* entity);

Cooldown cooldownDefault();

void cooldownSet(Cooldown cd, float time);

void cooldownRestart(Cooldown cd);

// Returns remaining time on cooldown in seconds
float cooldownGet(Cooldown cd);

Cooldown cooldownCreate(float time);
#endif
