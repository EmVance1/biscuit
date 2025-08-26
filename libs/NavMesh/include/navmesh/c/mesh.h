#ifndef NAVMESH_C_MESH_H
#define NAVMESH_C_MESH_H
#include "navmesh/c/shapes.h"


typedef struct navMesh navMesh;

typedef struct navPath {
    size_t count;
    navVector2f points[];
} navPath;


navMesh* navMesh_createFromFile(const char* filename, float scale);
void navMesh_writeToFile(const navMesh* self, const char* filename, float scale);
void navMesh_free(const navMesh* self);

size_t navMesh_getTriangleIndex(const navMesh* self, navVector2f p, float error);
navPath* navMesh_findPath(const navMesh* self, navVector2f begin, navVector2f end);


#endif
