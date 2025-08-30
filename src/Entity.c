#include "Entity.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "clock.h"
#include "pathtracker.h"


Entity Entity_createPlayer(sfVector2f position) {
    const sfVector2f size = (sfVector2f){ 20, 35 };

    return (Entity){
        .is_alive = true,
        .position  = position,
        .velocity  = (sfVector2f){ 0, 0 },
        .lastDir   = (sfVector2f){ 1, 0 },
        .rectBound = (sfFloatRect){position.x - size.x * 0.5f, position.y - size.y * 0.35f, size.x, size.y },
        .fillCol = sfWhite, // (sfColor){ 200, 200, 255, 255 },
        .speed = 3.f,
        .acc = 1.5f,
        .dashSpeed = 20.f,
        .health = 100,
        .meleeRange  = size.x * 2.5f,
        .meleeDamage = 40.f,
        .abilities = {true,true,true,true,true},
        .dashCooldown   = Cooldown_create(0.5f),
        .attackCooldown = Cooldown_create(0.3f),
        .stunCooldown   = Cooldown_create(2.0f),
        .hazardCooldown = Cooldown_create(0.5f),
        .fireballCooldown = Cooldown_create(2.0f),
        .attackAnim     = Cooldown_create(0.2f),
        .damageAnim     = Cooldown_create(0.05f),
        .pathtracker = NULL,
    };
}

Entity Entity_createEnemy(sfVector2f position, const navMesh* navmesh) {
    const sfVector2f size = (sfVector2f){ 20, 35 };

    return (Entity){
        .is_alive = true,
        .position  = position,
        .velocity  = (sfVector2f){ 0, 0 },
        .lastDir   = (sfVector2f){ 1, 0 },
        .rectBound = (sfFloatRect){position.x - size.x * 0.5f, position.y - size.y * 0.35f, size.x, size.y },
        .fillCol = (sfColor){ 255, 150, 150, 255 },
        .speed = 3.0f,
        .acc = 1.5f,
        .dashSpeed = 20.f,
        .health = 100,
        .meleeRange  = 70.f,
        .meleeDamage = 20.f,
        .abilities = {true,true,true,true,true},
        .dashCooldown   = Cooldown_create(0.5f),
        .attackCooldown = Cooldown_create(1.0f),
        .stunCooldown   = Cooldown_create(1.0f),
        .hazardCooldown = Cooldown_create(1.0f),
        .fireballCooldown = Cooldown_create(1.0f),
        .attackAnim     = Cooldown_create(0.2f),
        .damageAnim     = Cooldown_create(0.05f),
        .pathtracker = PathTracker_create(navmesh),
    };
}


static sfTexture* playerTexture;
static sfTexture* enemyTexture;

void Entity_loadTextures() {
    playerTexture = sfTexture_createFromFile("res/textures/player.png", NULL);
    enemyTexture = sfTexture_createFromFile("res/textures/enemy.png", NULL);
}

bool Entity_isEnemy(const Entity* entity) {
    return entity->pathtracker != NULL;
}

void Entity_kill(Entity* entity) {
    entity->is_alive = false;
    free(entity->pathtracker);
}


// Does not account for delta time
void Entity_offset(Entity* entity, sfVector2f dir) {
    const sfVector2f offset = dir;
    entity->position = sfVec2f_add(entity->position, offset);
    entity->rectBound.left += offset.x;
    entity->rectBound.top += offset.y;
}


// Accounts for delta time
void Entity_move(Entity* entity) {
    if (Cooldown_get(&entity->stunCooldown) > 0) return;

    if (Entity_isEnemy(entity)) {
        PathTracker_progress(entity->pathtracker);
    } else {
        const sfVector2f offset = sfVec2f_scale(entity->velocity, Clock_deltaTime() * 60.f);
        entity->position = sfVec2f_add(entity->position, offset);
        entity->rectBound.left += offset.x;
        entity->rectBound.top += offset.y;
        if (sfVec2f_lenSquared(entity->velocity) > 0.01f) {
            entity->lastDir = sfVec2f_norm(entity->velocity);
        }
    }
}

void Entity_addVelocity(Entity* entity, sfVector2f acceleration) {
    const float prevSpeedSq = sfVec2f_lenSquared(entity->velocity);
    entity->velocity = sfVec2f_add(entity->velocity, acceleration);

    if (sfVec2f_lenSquared(entity->velocity) > entity->speed * entity->speed) {
        if (!Cooldown_ready(&entity->dashCooldown)) {
            // dashing - alter direction and keep speed
            entity->velocity = sfVec2f_scale(sfVec2f_norm(entity->velocity), sqrtf(prevSpeedSq));
        } else {
            // regular movement - max out speed
            entity->velocity = sfVec2f_scale(sfVec2f_norm(entity->velocity), entity->speed);
        }
    }
}


void Entity_setVelocity(Entity* entity, sfVector2f velocity) {
    entity->velocity = velocity;
}

void Entity_startDash(Entity* entity) {
    if (Cooldown_ready(&entity->dashCooldown)) {
        sfVector2f velocity = entity->velocity;
        if (sfVec2f_lenSquared(velocity) < 0.001f) return;
        velocity = sfVec2f_norm(velocity);
        velocity = sfVec2f_scale(velocity, entity->dashSpeed);
        Entity_setVelocity(entity, velocity);
        Cooldown_reset(&entity->dashCooldown);
    }
}


void Entity_damage(Entity* entity, float damage) {
    entity->health -= damage;
    Cooldown_reset(&entity->damageAnim);
    // printf("Health: %.1f\n",entity->health);
    if (entity->health <= 0) {
        Entity_kill(entity);
    }
}


void Entity_render(sfRenderWindow* window, Entity *entity) {
    sfRectangleShape* rect = sfRectangleShape_create();
    sfRectangleShape_setSize(rect, (sfVector2f){ entity->rectBound.width, entity->rectBound.height });

    if (entity->lastDir.x < 0) {
        sfRectangleShape_setPosition(rect, (sfVector2f){ entity->rectBound.left + entity->rectBound.width, entity->rectBound.top });
        sfRectangleShape_setScale(rect, (sfVector2f){ -1.f, 1.f });
    } else {
        sfRectangleShape_setPosition(rect, (sfVector2f){ entity->rectBound.left, entity->rectBound.top });
    }

    if (Entity_isEnemy(entity)) {
        sfRectangleShape_setTexture(rect, enemyTexture, true);
    } else {
        sfRectangleShape_setTexture(rect, playerTexture, true);
    }

    if (!Cooldown_ready(&entity->damageAnim)) {
        sfRectangleShape_setFillColor(rect, sfRed);
    } else {
        sfRectangleShape_setFillColor(rect, entity->fillCol);
    }

    sfRenderWindow_drawRectangleShape(window, rect, NULL);
}


void Entity_stun(Entity* entity, float duration) {
    Cooldown_set(&entity->stunEffect, duration);
}


Cooldown Cooldown_create(float time) {
    return (Cooldown){ .cooldownBegin=Clock_totalTime() - time, .cooldownLength=time };
}

Cooldown Cooldown_default(void) {
    return Cooldown_create(1.f);
}

void Cooldown_reset(Cooldown* cd) {
    cd->cooldownBegin = Clock_totalTime();
}

void Cooldown_set(Cooldown* cd, float time) {
    cd->cooldownLength = time;
    cd->cooldownBegin = Clock_totalTime();
}

float Cooldown_get(const Cooldown* cd) {
    return cd->cooldownLength - (Clock_totalTime() - cd->cooldownBegin);
}

bool Cooldown_ready(const Cooldown* cd) {
    return (Clock_totalTime() - cd->cooldownBegin) >= cd->cooldownLength;
}



Projectile Projectile_free() {
    return (Projectile) {
        .free = true,
    };
}

Projectile Projectile_createFireball(sfVector2f _position, sfVector2f _velocity, float _collisionRadius, float _effectRadius) {
    return (Projectile) {
        .free = false,
        .projType = FIREBALL,
        .position = _position,
        .velocity = _velocity,
        .collisionRadius = _collisionRadius,
        .effectRadius = _effectRadius,
        .damage = 50.0f,
        .fillCol = sfRed,
        .texture = NULL,
    };
}

Projectile Projectile_createHazard(sfVector2f _position, float _effectRadius, float _duration) {
    return (Projectile) {
        .free = false,
        .projType = HAZARD,
        .position = _position,
        .velocity = (sfVector2f) {0,0},
        .collisionRadius = 0,
        .effectRadius = _effectRadius,
        .damage = 5.0f,
        .duration = Cooldown_create(_duration),
        .fillCol = sfGreen,
        .texture = NULL,
    };
}

void Projectile_move(Projectile* projectile) {
    const sfVector2f offset = sfVec2f_scale(projectile->velocity, Clock_deltaTime() * 60.f);
    projectile->position = sfVec2f_add(projectile->position, offset);
}


void Projectile_render(sfRenderWindow* window, Projectile* projectile) {
    if (projectile->free) return;
    sfCircleShape* circ = sfCircleShape_create();
    sfCircleShape_setPosition(circ, projectile->position);
    if (projectile->collisionRadius == 0) {
        sfCircleShape_setRadius(circ, projectile->effectRadius);
    } else {
        sfCircleShape_setRadius(circ, projectile->collisionRadius);
    }
    if (projectile->texture == NULL) {
        sfCircleShape_setFillColor(circ, projectile->fillCol);
    } else {
        sfCircleShape_setTexture(circ, projectile->texture, true);
    }
    sfRenderWindow_drawCircleShape(window, circ, NULL);
}
