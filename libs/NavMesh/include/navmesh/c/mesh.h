#ifndef NAVMESH_C_MESH_H
#define NAVMESH_C_MESH_H
#include "navmesh/c/shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct navMesh navMesh;

typedef struct navVertexChain {
    bool loop;
    size_t count;
    navVector2f points[];
} navVertexChain;

typedef struct navPolygonArray {
    size_t count;
    navVertexChain* polys[];
} navPolygonArray;


navMesh* navMesh_createFromFile(const char* filename, float scale);
void navMesh_writeToFile(const navMesh* self, const char* filename, float scale);
void navMesh_free(const navMesh* self);

size_t navMesh_getTriangleIndex(const navMesh* self, navVector2f p, float error);
const navVertexChain* navMesh_findPath(const navMesh* self, navVector2f begin, navVector2f end);

navPolygonArray* navMesh_clonePolygons(const navMesh* self);
void navMesh_freePolygons(const navPolygonArray* self);

#ifdef __cplusplus
}
#endif

#endif
