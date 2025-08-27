#ifndef PATHTRACKER_H
#define PATHTRACKER_H
#include <SFML/Graphics.h>
#include <stdbool.h>


typedef struct navMesh navMesh;
typedef struct navPath navPath;


typedef struct PathTracker PathTracker;
#define self PathTracker

#define PATHING_MAX_PENALTY 100000.f


PathTracker* PathTracker_create(const navMesh* mesh);
void PathTracker_free(const self*);

void  PathTracker_setSpeed(self*, float speed);
float PathTracker_getSpeed(const self*);

bool       PathTracker_setPosition(self*, sfVector2f position);
sfVector2f PathTracker_getPosition(const self*);

bool       PathTracker_setTargetPosition(self*, sfVector2f goal);
sfVector2f PathTracker_getTargetPosition(const self*);

const navPath* PathTracker_getActivePath(const self*);
float PathTracker_getActivePathLength(const self*);

size_t PathTracker_getCurrentIndex(const self*);
size_t PathTracker_getInverseIndex(const self*);

bool PathTracker_isMoving(const self*);
void PathTracker_pause(self*);
void PathTracker_stop (self*);
void PathTracker_start(self*);

void PathTracker_progress(self*);

#undef self

#endif
