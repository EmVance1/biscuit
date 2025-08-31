#ifndef ENTITY_H
#define ENTITY_H
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

    bool abilities[5];

    // ability cooldowns
    Cooldown dashCooldown;
    Cooldown attackCooldown;
    Cooldown stunCooldown;
    Cooldown fireballCooldown;
    Cooldown hazardCooldown;
    Cooldown attackAnim;
    Cooldown damageAnim;
    Cooldown stunEffect;

    float attackStartAngle;

    PathTracker* pathtracker;
    Cooldown pathCooldown;
} Entity;

typedef enum ProjectileType {
    FIREBALL,
    HAZARD,
} ProjectileType;

typedef struct Projectile {
    bool is_alive;
    bool is_dying;
    Cooldown impactTimer;
    ProjectileType projType;
    float collisionRadius;
    float effectRadius;
    sfVector2f position;
    sfVector2f velocity;
    float damage;
    Cooldown duration;
    Cooldown dot; // damage over time
    sfColor fillCol;
    sfTexture* texture;
} Projectile;

typedef enum Ability {
    A_ATTACK = 0,
    A_DASH = 1,
    A_FIREBALL = 2,
    A_STUN = 3,
    A_HAZARD = 4,
} Ability;

Entity Entity_createPlayer(sfVector2f position);
Entity Entity_createEnemy(sfVector2f position, const navMesh* navmesh, float meshtoworld);

void Entity_loadTextures();

void Entity_offset(Entity* entity, sfVector2f offset);
void Entity_move(Entity* entity, Entity* player, float meshtoworld);
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

void Projectile_kill(Projectile* self);
void Projectile_startKill(Projectile* self);
void Projectile_move(Projectile* projectile);
void Projectile_render(sfRenderWindow* window, Projectile* projectile);
Projectile Projectile_createFireball(sfVector2f _position, sfVector2f _velocity);
Projectile Projectile_createHazard(sfVector2f _position);

#endif
