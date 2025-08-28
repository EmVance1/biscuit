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

    sfColor fillCol;

    float speed;
    float acc;
    float dashSpeed;

    float health;
    float meleeRange;
    float meleeDamage;
    float stunRange;
    float stunDuration;

    // ability cooldowns
    Cooldown dashCooldown;
    Cooldown attackCooldown;
    Cooldown attackAnim;
    Cooldown damageAnim;
    Cooldown stunEffect;
    
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

void Entity_render(sfRenderWindow* window, Entity* entity);

Cooldown Cooldown_Default();

void Cooldown_Set(Cooldown cd, float time);

void Cooldown_Reset(Cooldown cd);

// Returns remaining time on cooldown in seconds
float Cooldown_Get(Cooldown cd);

Cooldown Cooldown_Create(float time);
#endif
