#include <SFML/Graphics.h>
#include <navmesh/c/lib.h>
#include <stdlib.h>
#include "pathtracker.h"
#include "clock.h"


static sfVector2f into_sf(navVector2f v) { return (sfVector2f){ v.x, v.y }; }
static navVector2f from_sf(sfVector2f v) { return (navVector2f){ v.x, v.y }; }


typedef struct PathTracker {
    const navMesh* mesh;
    navVertexChain* path;
    size_t path_index;
    float  path_prog;
    sfVector2f position;
    float speed;
    bool override_stop;
} PathTracker;


PathTracker* PathTracker_create(const navMesh* mesh) {
    PathTracker* self = (PathTracker*)malloc(sizeof(PathTracker));
    *self = (PathTracker) {
        .mesh = mesh,
        .speed = 1.f,
    };
    return self;
}

void PathTracker_free(const PathTracker* self) {
    free(self->path);
    free((PathTracker*)self);
}


void  PathTracker_setSpeed(PathTracker* self, float speed) {
    self->speed = speed;
}

float PathTracker_getSpeed(const PathTracker* self) {
    return self->speed;
}


bool PathTracker_setPosition(PathTracker* self, sfVector2f position) {
    const size_t idx = navMesh_getTriangleIndex(self->mesh, from_sf(position), 0.05f);
    if (idx == SIZE_MAX) { return false; }
    self->position = position;
    free(self->path);
    self->path = NULL;
    self->path_index = 0;
    self->path_prog = 0.f;
    return true;
}

sfVector2f PathTracker_getPosition(const PathTracker* self) {
    return self->position;
}


bool PathTracker_setTargetPosition(PathTracker* self, sfVector2f goal) {
    free(self->path);
    self->path = navMesh_findPath(self->mesh, from_sf(self->position), from_sf(goal));
    if (!self->path) { return false; }
    self->path_index = 0;
    self->path_prog = 0;
    return true;
}

sfVector2f PathTracker_getTargetPosition(const PathTracker* self) {
    if (self->path) {
        return into_sf(self->path->points[self->path->count - 1]);
    } else {
        return self->position;
    }
}


const navVertexChain* PathTracker_getActivePath(const PathTracker* self) {
    return self->path;
}

float PathTracker_getActivePathLength(const PathTracker* self) {
    float result = 0.f;
    for (size_t i = 0; i < self->path->count - 1; i++) {
        result += navVec2f_len(navVec2f_sub(self->path->points[i], self->path->points[i+1]));
    }
    return result;
}


bool PathTracker_isMoving(const PathTracker* self) {
    return (self->path && (self->path_index != self->path->count - 1) && !self->override_stop);
}

void PathTracker_pause(PathTracker* self) {
    self->override_stop = true;
}

void PathTracker_stop (PathTracker* self) {
    self->override_stop = true;
    free(self->path);
    self->path = NULL;
    self->path_index = 0;
    self->path_prog = 0;
}

void PathTracker_start(PathTracker* self) {
    self->override_stop = false;
}


void PathTracker_progress(PathTracker* self) {
    if (!PathTracker_isMoving(self)) { return; }

    if (sfVec2f_cmp(into_sf(self->path->points[self->path_index + 1]), self->position) < 0.001f) {
        if (++self->path_index == self->path->count - 1) { return; }
    }
    const sfVector2f diff = sfVec2f_sub(into_sf(self->path->points[self->path_index + 1]), self->position);
    const float dist = sfVec2f_len(diff);
    const float actual = self->speed * Clock_deltaTime() * 60.f;
    const sfVector2f dir = sfVec2f_scale(diff, (1.f / dist));
    if (dist < actual) {
        self->path_index++;
        self->position = into_sf(self->path->points[self->path_index]);
        self->position = sfVec2f_add(self->position, sfVec2f_scale(dir, actual - dist));
    } else {
        self->position = sfVec2f_add(self->position, sfVec2f_scale(dir, actual));
    }
}

