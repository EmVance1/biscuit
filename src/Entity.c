#include "Entity.h"
#include <math.h>
#include <stdio.h>
#include "clock.h"


// Does not account for delta time
void Entity_offset(Entity* entity, sfVector2f dir) {
    sfVector2f offset = dir;
    entity->position = sfVec2f_add(entity->position, offset);
    entity->rectBound.left += offset.x;
    entity->rectBound.top += offset.y;
}

// Accounts for delta time
void Entity_move(Entity* entity) {
    sfVector2f offset = sfVec2f_scale(entity->velocity,Clock_deltaTime());
    entity->position = sfVec2f_add(entity->position, offset);
    entity->rectBound.left += offset.x;
    entity->rectBound.top += offset.y;
    if (sfVec2f_len(entity->velocity) > 0.01f) {
        entity->lastDir = sfVec2f_norm(entity->velocity);
    }
}

void Entity_addVelocity(Entity* entity, sfVector2f acceleration) {  
    float prevSpeed = sfVec2f_len(entity->velocity);
    entity->velocity = sfVec2f_add(entity->velocity,acceleration);

    if (sfVec2f_len(entity->velocity) > entity->speed) {
        // moved from slower into greater than max speed
        if (prevSpeed < entity->speed) {
            entity->velocity = sfVec2f_scale(entity->velocity,entity->speed/sfVec2f_len(entity->velocity));
        // was already above max speed, alter direction and keep speed (because of dash)
        } else {
            entity->velocity = sfVec2f_scale(entity->velocity,prevSpeed/sfVec2f_len(entity->velocity));
        }
    }
}

void Entity_setVelocity(Entity* entity, sfVector2f velocity) {
    entity->velocity = velocity;
}

static float getCooldown(Cooldown cooldown) {
    return cooldown.cooldownLength - sfTime_asSeconds(sfClock_getElapsedTime(cooldown.clock));
}

void Entity_startDash(Entity* entity) {
    if (getCooldown(entity->dashCooldown) <= 0.0f) {
        sfVector2f velocity = entity->velocity;
        if (sfVec2f_len(velocity) == 0) return;
        velocity = sfVec2f_norm(velocity);
        velocity = sfVec2f_scale(velocity, entity->dashSpeed);
        Entity_setVelocity(entity, velocity);
        sfClock_restart(entity->dashCooldown.clock);
    }
}

void Entity_damage(Entity* entity, float damage) {
    entity->health -= damage;
    Cooldown_Reset(entity->damageAnim);
    if (entity->health <= 0) Entity_kill(entity);
}

void Entity_kill(Entity* entity) {
}

void Entity_updateVelocity(Entity* entity) {
    // decelerate entity by 10% of current velocity
    sfVector2f oppositeVel = sfVec2f_scale(entity->velocity, -0.1f);
    Entity_setVelocity(entity, oppositeVel);
}

void Entity_render(sfRenderWindow* window, Entity *entity) {
    sfRectangleShape* rect = sfRectangleShape_create();
    sfRenderStates renderState = sfRenderStates_default();
    sfRectangleShape_setPosition(rect, (sfVector2f) {entity->rectBound.left, entity->rectBound.top});
    sfRectangleShape_setSize(rect, (sfVector2f) {entity->rectBound.width, entity->rectBound.height});

    if (entity->health <= 0) {
       return; 
    } else if (Cooldown_Get(entity->damageAnim) > 0) {
        sfRectangleShape_setFillColor(rect, sfRed);
    } else {
        sfRectangleShape_setFillColor(rect, entity->fillCol);
    }

    sfRenderWindow_drawRectangleShape(window, rect, &renderState);
}

Cooldown Cooldown_Default() {
    return (Cooldown) {sfClock_create(), 1.0f};
}

float Cooldown_Get(Cooldown cd) {
    return cd.cooldownLength - sfTime_asSeconds(sfClock_getElapsedTime(cd.clock));
}

void Cooldown_Set(Cooldown cd, float time) {
    cd.cooldownLength = time;
    sfClock_restart(cd.clock);
}

void Cooldown_Reset(Cooldown cd) {
    sfClock_restart(cd.clock);
}

Cooldown Cooldown_Create(float time) {
    return (Cooldown) {sfClock_create(), time};
}
