#include "Entity.h"


void moveEntity(Entity* entity, sfVector2f offset) {
    entity->position = sfVec2f_add(entity->position, offset);
    entity->rectBound.left += offset.x;
    entity->rectBound.top += offset.y;
}
