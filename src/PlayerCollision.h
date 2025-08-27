#ifndef PLAYER_COLLISION_H
#define PLAYER_COLLISION_H
#include <SFML/Graphics.h>
#include <stdbool.h>
#include "Entity.h"


bool rectLineCollision(sfFloatRect rect, sfVector2f v0, sfVector2f v1);

sfVector2f resolveRectLineCollision(sfFloatRect rect, sfVector2f* v, int lines);

bool handlePlayerWallCollision(Entity* player, sfVector2f* vertices, int numVertices);


#endif
