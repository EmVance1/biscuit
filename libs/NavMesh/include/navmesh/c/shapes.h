#ifndef NAVMESH_C_SHAPES_H
#define NAVMESH_C_SHAPES_H
#include <stdbool.h>
#include <math.h>


typedef struct navVector2f {
    float x;
    float y;
} navVector2f;

navVector2f navVec2f_add(navVector2f a, navVector2f b);
navVector2f navVec2f_sub(navVector2f a, navVector2f b);
navVector2f navVec2f_scale(navVector2f a, float b);
float navVec2f_dot(navVector2f a, navVector2f b);
float navVec2f_len(navVector2f v);
navVector2f navVec2f_norm(navVector2f v);
navVector2f navVec2f_compDiv(navVector2f a, navVector2f b);
navVector2f navVec2f_compDivFloor(navVector2f a, navVector2f b);
navVector2f navVec2f_compMul(navVector2f a, navVector2f b);
navVector2f navVec2f_perpCw(navVector2f v);
navVector2f navVec2f_perpCcw(navVector2f v);
float navVec2f_cmp(navVector2f a, navVector2f b);
float navVec2f_angle(navVector2f a, navVector2f b);


typedef struct navVector2i {
    int x;
    int y;
} navVector2i;

navVector2i navVec2i_add(navVector2i a, navVector2i b);
navVector2i navVec2i_sub(navVector2i a, navVector2i b);
navVector2f navVec2i_scale(navVector2i a, float b);
int   navVec2i_dot(navVector2i a, navVector2i b);
float navVec2i_len(navVector2i v);
navVector2f navVec2i_norm(navVector2i v);
navVector2f navVec2i_compDiv(navVector2i a, navVector2i b);
navVector2i navVec2i_compDivFloor(navVector2i a, navVector2i b);
navVector2i navVec2i_compMul(navVector2i a, navVector2i b);
navVector2i navVec2i_perpCw(navVector2i v);
navVector2i navVec2i_perpCcw(navVector2i v);
int   navVec2i_cmp(navVector2i a, navVector2i b);
float navVec2i_angle(navVector2i a, navVector2i b);



typedef struct navCircle {
    navVector2f pos;
    float radius;
} navCircle;

bool navCircle_contains(const navCircle* self, navVector2f p);


typedef struct navTriangle {
    size_t A;
    size_t B;
    size_t C;
    float weight;
} navTriangle;

navVector2f navTriangle_centroid(const navTriangle* self, const navVector2f* vertices);
navVector2f navTriangle_circumcenter(const navTriangle* self, const navVector2f* vertices);
bool navTriangle_contains(const navTriangle* self, const navVector2f* vertices, navVector2f p, bool with_corners);
bool navTriangle_contains_with_error(const navTriangle* self, const navVector2f* vertices, navVector2f p, float error);


#endif
