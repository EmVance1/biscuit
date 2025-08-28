#include <stdlib.h>
#include "PlayerCollision.h"
#include "Entity.h"


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

sfVector2f Collision_ResolveRectLine(sfFloatRect rect, sfVector2f* v, int lines) {
    // collect distance to offset from rect and line
    float* lineOffsets = (float*)calloc(lines, sizeof(float));
    sfVector2f* offsetDir = (sfVector2f*)calloc(lines, sizeof(sfVector2f));
    for (int i=0; i<lines; i++) {
        sfVector2f v0 = v[2*i+0];
        sfVector2f v1 = v[2*i+1];
        if (v0.y == v1.y) {
            lineOffsets[i] = v0.x < v1.x ? v0.y-rect.top : rect.top+rect.height-v0.y;
            sfVector2f up = {0,-1};
            sfVector2f down = {0,1};
            offsetDir[i] = v0.x < v1.x ? down : up;
        } else if (v0.x == v1.x) {
            lineOffsets[i] = v0.y < v1.y ? rect.left+rect.width - v0.x : v0.x - rect.left;
            sfVector2f left = {-1,0};
            sfVector2f right = {1,0};
            offsetDir[i] = v0.y < v1.y ? left : right;
        }
    }
    if (lines == 0) return (sfVector2f) {0,0};

    // find min offset
    float minOffset = 1000000.0f;
    sfVector2f offsetVector;
    for (int i=0; i<lines; i++) {
        if (lineOffsets[i] < minOffset) {
            minOffset = lineOffsets[i];
            offsetVector = sfVec2f_scale(offsetDir[i],minOffset);
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
        if (offset.x == 0 && offset.y == 0) return false;
    } else return false;

    free(collisionVertices);

    return true;
}

bool Collision_HandlePlayerNavmesh(Entity* player, navPolygonArray* meshPolys) {
    sfFloatRect rectBound = player->rectBound;
    for (int k=0; k<(int) meshPolys->count; k++) {
        int numVertices = meshPolys->polys[k]->count;
        int iters = meshPolys->polys[k]->loop ? numVertices : numVertices-1; // In case of loop, have to go one line further than numVertices
        sfVector2f* collisionVertices = (sfVector2f*)malloc(sizeof(sfVector2f)* (numVertices+1) *2);
        int lines = 0;

        for (int i=0; i<iters; i++) {
            navVector2f nav0 = meshPolys->polys[k]->points[i];
            navVector2f nav1 = meshPolys->polys[k]->points[(i+1)%numVertices];
            sfVector2f v0 = (sfVector2f) {nav0.x,nav0.y};
            sfVector2f v1 = (sfVector2f) {nav1.x,nav1.y};
            if (Collision_RectLine(rectBound, v0, v1)) {
                collisionVertices[2*lines+0] = v0;
                collisionVertices[2*lines+1] = v1;
                lines++;
            }
        }
        if (lines > 0) {
            sfVector2f offset = Collision_ResolveRectLine(rectBound, collisionVertices, lines);
            Entity_offset(player, offset);
            if (offset.x == 0 && offset.y == 0) return false;
        } else {
            free(collisionVertices);
            return false;
        }

        free(collisionVertices);

        return true;
    }
    return false;
}
