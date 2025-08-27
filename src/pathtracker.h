#ifndef PATHTRACKER_H
#define PATHTRACKER_H
#include <SFML/Graphics.h>
#include <navmesh/c/lib.h>


#define PATHING_MAX_PENALTY 100000.f


typedef struct PathTracker PathTracker;


PathTracker* PathTracker_create(const navMesh* mesh);
void PathTracker_free(const PathTracker* self);

void  PathTracker_setSpeed(PathTracker* self, float speed);
float PathTracker_getSpeed(const PathTracker* self);

bool       PathTracker_setPosition(PathTracker* self, sfVector2f position);
sfVector2f PathTracker_getPosition(const PathTracker* self);

bool       PathTracker_setTargetPosition(PathTracker* self, sfVector2f goal);
sfVector2f PathTracker_getTargetPosition(const PathTracker* self);

const navPath* PathTracker_getActivePath(PathTracker* self);
float PathTracker_getActivePathLength(const PathTracker* self);

size_t PathTracker_getCurrentIndex(const PathTracker* self);
size_t PathTracker_getInverseIndex(const PathTracker* self);

bool PathTracker_isMoving(const PathTracker* self);
void PathTracker_pause(PathTracker* self);
void PathTracker_stop (PathTracker* self);
void PathTracker_start(PathTracker* self);

void progress();


#endif
