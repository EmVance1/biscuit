#ifndef PLAYER_COLLISION_H
#define PLAYER_COLLISION_H
#include <SFML/Graphics.h>
#include <stdbool.h>
#include "Entity.h"
#include "navmesh/c/mesh.h"


bool Collision_RectLine(sfFloatRect rect, sfVector2f v0, sfVector2f v1);

sfVector2f Collision_ResolveRectLine(sfFloatRect rect, sfVector2f* v, int lines);

bool Collision_HandlePlayerWall(Entity* player, sfVector2f* vertices, int numVertices);

bool Collision_HandlePlayerNavmesh(Entity* player, navPolygonArray* meshPoly);

#endif
