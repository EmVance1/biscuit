#include "Entity.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "clock.h"
#include "pathtracker.h"


Entity Entity_createPlayer(sfVector2f position) {
    const sfVector2f size = (sfVector2f){ 15, 25 };

    return (Entity){
        .is_alive = true,
        .position  = position,
        .velocity  = (sfVector2f){ 0, 0 },
        .lastDir   = (sfVector2f){ 1, 0 },
        .rectBound = (sfFloatRect){position.x - size.x * 0.5f, position.y - size.y * 0.5f, size.x, size.y },
        .fillCol = sfBlue,
        .speed = 3.f,
        .acc = 1.5f,
        .dashSpeed = 16.f,
        .health = 100,
        .meleeRange  = size.x * 2.5f,
        .meleeDamage = 40.f,
        .dashCooldown   = Cooldown_create(0.5f),
        .attackCooldown = Cooldown_create(0.3f),
        .attackAnim     = Cooldown_create(0.2f),
        .damageAnim     = Cooldown_create(0.05f),
        .pathtracker = NULL,
    };
}

Entity Entity_createEnemy(sfVector2f position, sfColor color, const navMesh* navmesh) {
    const sfVector2f size = (sfVector2f){ 15, 25 };

    return (Entity){
        .is_alive = true,
        .position  = position,
        .velocity  = (sfVector2f){ 0, 0 },
        .lastDir   = (sfVector2f){ 1, 0 },
        .rectBound = (sfFloatRect){position.x - size.x * 0.5f, position.y - size.y * 0.5f, size.x, size.y },
        .fillCol = color,
        .speed = 3.0f,
        .acc = 1.5f,
        .dashSpeed = 16.f,
        .health = 100,
        .meleeRange  = 70.f,
        .meleeDamage = 20.f,
        .dashCooldown   = Cooldown_create(0.5f),
        .attackCooldown = Cooldown_create(1.0f),
        .attackAnim     = Cooldown_create(0.2f),
        .damageAnim     = Cooldown_create(0.05f),
        .pathtracker = PathTracker_create(navmesh),
    };
}


bool Entity_isEnemy(const Entity* entity) {
    return entity->pathtracker != NULL;
}

void Entity_kill(Entity* entity) {
    entity->is_alive = false;
    free(entity->pathtracker);
    Cooldown_free(&entity->dashCooldown);
    Cooldown_free(&entity->attackCooldown);
    Cooldown_free(&entity->attackAnim);
    Cooldown_free(&entity->damageAnim);
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
    if (entity->health <= 0) {
        Entity_kill(entity);
    }
}


// confused about this
/*
void Entity_updateVelocity(Entity* entity) {
    // decelerate entity by 10% of current velocity
    sfVector2f oppositeVel = sfVec2f_scale(entity->velocity, -0.1f);
    Entity_setVelocity(entity, oppositeVel);
}
*/


void Entity_render(sfRenderWindow* window, Entity *entity) {
    sfRectangleShape* rect = sfRectangleShape_create();
    sfRectangleShape_setPosition(rect, (sfVector2f){ entity->rectBound.left, entity->rectBound.top });
    sfRectangleShape_setSize(rect, (sfVector2f){ entity->rectBound.width, entity->rectBound.height });

    if (!Cooldown_ready(&entity->damageAnim)) {
        sfRectangleShape_setFillColor(rect, sfRed);
    } else {
        sfRectangleShape_setFillColor(rect, entity->fillCol);
    }

    sfRenderWindow_drawRectangleShape(window, rect, NULL);
}



Cooldown Cooldown_create(float time) {
    return (Cooldown){ sfClock_create(), time };
}

Cooldown Cooldown_default() {
    return Cooldown_create(1.f);
}

void Cooldown_free(const Cooldown* self) {
    sfClock_destroy(self->clock);
}

void Cooldown_reset(Cooldown* cd) {
    sfClock_restart(cd->clock);
}

void Cooldown_set(Cooldown* cd, float time) {
    cd->cooldownLength = time;
    sfClock_restart(cd->clock);
}

float Cooldown_get(const Cooldown* cd) {
    return cd->cooldownLength - sfTime_asSeconds(sfClock_getElapsedTime(cd->clock));
}

bool Cooldown_ready(const Cooldown* cd) {
    return sfTime_asSeconds(sfClock_getElapsedTime(cd->clock)) >= cd->cooldownLength;
}

