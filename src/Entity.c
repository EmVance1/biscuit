#include "Entity.h"
#include "SFML/System/Clock.h"
#include "SFML/System/Time.h"
#include "SFML/System/Vector2.h"
#include "clock.h"


void moveEntity(Entity* entity, sfVector2f dir) {
    sfVector2f offset = sfVec2f_scale(dir,Clock_deltaTime());
    entity->position = sfVec2f_add(entity->position, offset);
    entity->rectBound.left += offset.x;
    entity->rectBound.top += offset.y;
}

void addVelocity(Entity* entity, sfVector2f velocity) {
    entity->velocity = sfVec2f_add(entity->velocity,velocity);
    
    // clamp velocity to not exceed entity speed limit
    float speed = sfVec2f_len(entity->velocity);
    if (speed > entity->speed) {
        sfVector2f newVel = sfVec2f_norm(entity->velocity);
        newVel = sfVec2f_scale(newVel, entity->speed);
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
