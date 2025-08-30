#include <stdlib.h>
#include <stdio.h>
#include "PlayerCollision.h"
#include "Entity.h"
#include "SFML/System/Vector2.h"


bool Collision_RectLine(sfFloatRect rect, sfVector2f v0, sfVector2f v1) {
    if (v0.y == v1.y) {
        float left = v0.x < v1.x ? v0.x : v1.x;
        float right = v0.x < v1.x ? v1.x : v0.x;
        float top = v0.y;
        // both edges are entirely left or entirely right
        if ((rect.left < left && rect.left+rect.width < left) || (rect.left > right && rect.left+rect.width > right)) {
            return false;
        }
        // top edge is above and bottom edge is below line
        return (rect.top < top && top < rect.top + rect.height);
    } else {
        float top = v0.y < v1.y ? v0.y : v1.y;
        float bottom = v0.y < v1.y ? v1.y : v0.y;
        float left = v0.x;
        // both edges are entirely above or below
        if ((rect.top < top && rect.top+rect.height < top) || (rect.top > bottom && rect.top+rect.height > bottom)) {
            return false;
        }
  // left edge is left and right edge is right of line
        return (rect.left < left && left < rect.left+rect.width);
    }
    return false;
}

sfVector2f Collision_ResolveRectLine(sfFloatRect rect, sfVector2f* vertices, int lines) {
    // collect distance to offset from rect and line
    float* lineOffsets = (float*)calloc(lines, sizeof(float));
    sfVector2f* offsetDir = (sfVector2f*)calloc(lines, sizeof(sfVector2f));

    for (int i = 0; i < lines; i++) {
        sfVector2f v0 = vertices[2*i+0];
        sfVector2f v1 = vertices[2*i+1];
        if (fabsf(v0.y - v1.y) < 0.001f) {
            lineOffsets[i] = v0.x < v1.x ? v0.y-rect.top : rect.top+rect.height-v0.y;
            sfVector2f up   = (sfVector2f){ 0, -1 };
            sfVector2f down = (sfVector2f){ 0,  1 };
            offsetDir[i] = v0.x < v1.x ? down : up;
        } else if (fabsf(v0.x - v1.x) < 0.001f) {
            lineOffsets[i] = v0.y < v1.y ? rect.left+rect.width - v0.x : v0.x - rect.left;
            sfVector2f left  = (sfVector2f){ -1, 0 };
            sfVector2f right = (sfVector2f){  1, 0 };
            offsetDir[i] = v0.y < v1.y ? left : right;
        }
    }
    if (lines == 0) return (sfVector2f){ 0, 0 };

    // find min offset
    float minOffset = 1000000.0f;
    sfVector2f offsetVector;
    for (int i = 0; i < lines; i++) {
        if (lineOffsets[i] < minOffset) {
            minOffset = lineOffsets[i];
            offsetVector = sfVec2f_scale(offsetDir[i], minOffset);
            if (offsetDir[i].x != 0) offsetVector.x += 1*offsetDir[i].x;
            if (offsetDir[i].y != 0) offsetVector.y += 1*offsetDir[i].y;
        }
    }

    free(lineOffsets);
    free(offsetDir);

    return offsetVector;
}

bool Collision_HandlePlayerWall(Entity* player, sfVector2f* vertices, int numVertices) {
    sfFloatRect rectBound = player->rectBound;
    sfVector2f* collisionVertices = (sfVector2f*)malloc(sizeof(sfVector2f)* numVertices *2);
    int lines = 0;
    for (int i=0; i<numVertices-1; i++) {
        sfVector2f v0 = vertices[i];
        sfVector2f v1 = vertices[i+1];
        if (Collision_RectLine(rectBound, v0, v1)) {
            collisionVertices[2*lines+0] = v0;
            collisionVertices[2*lines+1] = v1;
            lines++;
        }
    }
    if (lines > 0) {
        sfVector2f offset = Collision_ResolveRectLine(rectBound, collisionVertices, lines);
        Entity_offset(player, offset);
        if (sfVec2f_lenSquared(offset) < 0.001f) return false;
    } else return false;

    free(collisionVertices);

    return true;
}

bool Collision_HandlePlayerNavmesh(Entity* player, navPolygonArray* meshPolys, float worldscale) {
    sfFloatRect rectBound = player->rectBound;
    for (size_t k = 0; k < meshPolys->count; k++) {
        size_t numVertices = meshPolys->polys[k]->count;
        size_t iters = meshPolys->polys[k]->loop ? numVertices : numVertices-1; // In case of loop, have to go one line further than numVertices
        sfVector2f* collisionVertices = (sfVector2f*)malloc(sizeof(sfVector2f) * (numVertices+1) * 2);
        if (!collisionVertices) return false;
        int lines = 0;

        for (size_t i = 0; i < iters; i++) {
            navVector2f nav0 = meshPolys->polys[k]->points[i];
            navVector2f nav1 = meshPolys->polys[k]->points[(i+1)%numVertices];

            sfVector2f v0 = (sfVector2f) {nav0.x*worldscale,nav0.y*worldscale};
            sfVector2f v1 = (sfVector2f) {nav1.x*worldscale,nav1.y*worldscale};

            if (Collision_RectLine(rectBound, v0, v1)) {
                collisionVertices[2*lines+0] = v0;
                collisionVertices[2*lines+1] = v1;
                lines++;
            }
        }
        if (lines > 0) {
            sfVector2f offset = Collision_ResolveRectLine(rectBound, collisionVertices, lines);
            Entity_offset(player, offset);
            if (sfVec2f_lenSquared(offset) <= 0.001f) return false;
        } else {
            free(collisionVertices);
            return false;
        }

        free(collisionVertices);
        return true;
    }

    return false;
}


float Collision_distanceSqPointLineSeg(sfVector2f p, sfVector2f v0, sfVector2f v1) {
    sfVector2f v0p = sfVec2f_sub(p,v0);
    sfVector2f v0v1 = sfVec2f_sub(v1,v0);
    float dot = sfVec2f_dot(v0p,v0v1);
    float sqMag = sfVec2f_lenSquared(v0v1);
    float t = dot/sqMag;
    float trueT = fmaxf(fminf(t,1),0);
    sfVector2f st = sfVec2f_add(v0,sfVec2f_scale(v0v1,trueT));
    return sfVec2f_lenSquared(sfVec2f_sub(st,p));
}


bool Collision_ProjectileWallNavmesh(Projectile* projectile, navPolygonArray* meshPolys, float worldscale) { 
    float collRadiusSq = projectile->collisionRadius*projectile->collisionRadius;
    for (size_t k = 0; k < meshPolys->count; k++) {
        size_t numVertices = meshPolys->polys[k]->count;
        size_t iters = meshPolys->polys[k]->loop ? numVertices : numVertices-1; // In case of loop, have to go one line further than numVertices
        for (size_t i = 0; i < iters; i++) {
            navVector2f nav0 = meshPolys->polys[k]->points[i];
            navVector2f nav1 = meshPolys->polys[k]->points[(i+1)%numVertices];

            sfVector2f v0 = (sfVector2f) {nav0.x*worldscale,nav0.y*worldscale};
            sfVector2f v1 = (sfVector2f) {nav1.x*worldscale,nav1.y*worldscale};

            if (Collision_distanceSqPointLineSeg(projectile->position, v0, v1) <= collRadiusSq) {
                return true;
            }
        }
    }
    return false;
}


bool Collision_ProjectileWall(Projectile* projectile, sfVector2f* points, int count, int loop) { 
    float collRadiusSq = projectile->collisionRadius*projectile->collisionRadius;
    for (int k=0; k<(int) count; k++) {
        int numVertices = count;
        int iters = loop ? numVertices : numVertices-1; // In case of loop, have to go one line further than numVertices

        for (int i=0; i<iters; i++) {
            sfVector2f v0 = points[i];
            sfVector2f v1 = points[(i+1)%numVertices];
            if (Collision_distanceSqPointLineSeg(projectile->position, v0, v1) <= collRadiusSq) {
                return true;
            }
        }
    }
    return false;
}
