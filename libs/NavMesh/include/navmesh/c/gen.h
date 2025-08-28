#ifndef NAVMESH_C_GEN_H
#define NAVMESH_C_GEN_H
#include "navmesh/c/mesh.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool(*navPredicate)(const uint8_t*);

typedef enum navMethod {
    GEN_METHOD_MARCHING_SQUARES,
    GEN_METHOD_FLOODFILL,
} navMethod;

navMesh* navMesh_createFromGrid(
        const uint8_t* grid,
        size_t width,
        size_t height,
        size_t stride,
        size_t index,
        navMethod method,
        float epsilon
    );

/*
navMesh navMesh_createFromShapes(
        const navVector2f** polys,
        size_t polycount,
        const navCircle* circles,
        size_t circlecount,
        const navVector2f* fillers,
        size_t fillercount,
        uint32_t circle_res,
        float epsilon
    );
*/


#ifdef __cplusplus
}
#endif

#endif
