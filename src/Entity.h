#ifndef ENTITY_H
#define ENTITY_H
#include "SFML/System/Vector2.h"
#include <SFML/Graphics.h>
#include <stdbool.h>


typedef struct PathTracker PathTracker;
typedef struct navMesh navMesh;

typedef struct Cooldown {
    float cooldownBegin;
    float cooldownLength;
} Cooldown;

typedef struct Entity {
    bool is_alive;

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
    Cooldown stunCooldown;
    Cooldown fireballCooldown;
    Cooldown attackAnim;
    Cooldown damageAnim;
    Cooldown stunEffect;
    float attackStartAngle;

    PathTracker* pathtracker;
} Entity;

typedef enum ProjectileType {
    FIREBALL,
} ProjectileType;

typedef struct Projectile {
    bool free;
    ProjectileType projType;
    float collisionRadius;
    float effectRadius;
    sfVector2f position;
    sfVector2f velocity;
} Projectile;


Entity Entity_createPlayer(sfVector2f position);
Entity Entity_createEnemy(sfVector2f position, sfColor color, const navMesh* navmesh);

bool Entity_isEnemy(const Entity* entity);
void Entity_offset(Entity* entity, sfVector2f offset);
void Entity_move(Entity* entity);
void Entity_startDash(Entity* entity);
void Entity_addVelocity(Entity* entity, sfVector2f acceleration);
void Entity_setVelocity(Entity* entity, sfVector2f velocity);
void Entity_damage(Entity* entity, float damage);
void Entity_kill(Entity* entity);
void Entity_render(sfRenderWindow* window, Entity* entity);
void Entity_stun(Entity* entity, float duration);

Cooldown Cooldown_create(float time);
Cooldown Cooldown_default(void);
void Cooldown_reset(Cooldown* cd);
void Cooldown_set(Cooldown* cd, float time);
// Returns remaining time on cooldown in seconds
float Cooldown_get(const Cooldown* cd);
bool Cooldown_ready(const Cooldown* cd);

Projectile Projectile_free();
Projectile Projectile_createFireball(sfVector2f _position, sfVector2f _velocity, float _collisionRadius, float _effectRadius);
void Projectile_move(Projectile* projectile);
void Projectile_render(sfRenderWindow* window, Projectile* projectile);

#endif
