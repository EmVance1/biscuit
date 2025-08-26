#include "Entity.h"
#include "clock.h"
#include <math.h>
#include <stdio.h>


// Does not account for delta time
void moveEntity(Entity* entity, sfVector2f dir) {
    sfVector2f offset = dir;
    entity->position = sfVec2f_add(entity->position, offset);
    entity->rectBound.left += offset.x;
    entity->rectBound.top += offset.y;
}

void addVelocity(Entity* entity, sfVector2f acceleration) {  
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

void setVelocity(Entity* entity, sfVector2f velocity) {
    entity->velocity = velocity;
}

static float getCooldown(Cooldown cooldown) {
    return cooldown.cooldownLength - sfTime_asSeconds(sfClock_getElapsedTime(cooldown.clock));
}

void startDash(Entity* entity) {
    if (getCooldown(entity->dashCooldown) <= 0.0f) {
        sfVector2f velocity = entity->velocity;
        if (sfVec2f_len(velocity) == 0) return;
        velocity = sfVec2f_norm(velocity);
        velocity = sfVec2f_scale(velocity, entity->dashSpeed);
        setVelocity(entity, velocity);
        sfClock_restart(entity->dashCooldown.clock);
    }
}

void updateVelocity(Entity* entity) {
    // decelerate entity by 10% of current velocity
    sfVector2f oppositeVel = sfVec2f_scale(entity->velocity, -0.1f);
    setVelocity(entity, oppositeVel);
}
