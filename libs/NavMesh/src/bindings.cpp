#define NAVMESH_GENERATION
#include "navmesh/c/lib.h"
#include "navmesh/lib.h"


static nav::Vector2f fromc(navVector2f v) { return nav::Vector2f{ v.x, v.y }; }
static navVector2f intoc(nav::Vector2f v) { return navVector2f{ v.x, v.y }; }

#ifdef __cplusplus
extern "C" {
#endif

navVector2f navVec2f_add(navVector2f a, navVector2f b) { return navVector2f{ a.x + b.x, a.y + b.y }; }
navVector2f navVec2f_sub(navVector2f a, navVector2f b) { return navVector2f{ a.x - b.x, a.y - b.y }; }
navVector2f navVec2f_scale(navVector2f a, float b) { return navVector2f{ a.x * b, a.y * b }; }
float navVec2f_dot(navVector2f a, navVector2f b) { return a.x * b.x + a.y * b.y; }
float navVec2f_len(navVector2f v) { return std::sqrtf(navVec2f_dot(v, v)); }
navVector2f navVec2f_norm(navVector2f v) { return navVec2f_scale(v, 1.f / navVec2f_len(v)); }
navVector2f navVec2f_compDiv(navVector2f a, navVector2f b) { return navVector2f{ a.x / b.x, a.y / b.y }; }
navVector2f navVec2f_compMul(navVector2f a, navVector2f b) { return navVector2f{ a.x * b.x, a.y * b.y }; }
navVector2f navVec2f_perpCw(navVector2f v)   { return navVector2f{ v.y, -v.x }; }
navVector2f navVec2f_perpCcw(navVector2f v)  { return navVector2f{ -v.y, v.x }; }
float navVec2f_cmp(navVector2f a, navVector2f b)   { const float c = std::fabsf(a.x - b.x); const float d = std::fabsf(a.y - b.y); return c > d ? c : d; }
float navVec2f_angle(navVector2f a, navVector2f b) { const float c = navVec2f_dot(a, b); const float d = navVec2f_len(a) * navVec2f_len(b); return std::acos(c / d); }

navVector2i navVec2i_add(navVector2i a, navVector2i b) { return navVector2i{ a.x + b.x, a.y + b.y }; }
navVector2i navVec2i_sub(navVector2i a, navVector2i b) { return navVector2i{ a.x - b.x, a.y - b.y }; }
navVector2f navVec2i_scale(navVector2i a, float b) { return navVector2f{ (float)a.x * b, (float)a.y * b }; }
int   navVec2i_dot(navVector2i a, navVector2i b) { return a.x * b.x + a.y * b.y; }
float navVec2i_len(navVector2i v) { return navVec2f_len(navVector2f{ (float)v.x, (float)v.y }); }
navVector2f navVec2i_norm(navVector2i v) { return navVec2i_scale(v, 1.f / navVec2i_len(v)); }
navVector2f navVec2i_compDiv(navVector2i a, navVector2i b) { return navVector2f{ (float)a.x / (float)b.x, (float)a.y / (float)b.y }; }
navVector2i navVec2i_compDivFloor(navVector2i a, navVector2i b) { return navVector2i{ a.x / b.x, a.y / b.y }; }
navVector2i navVec2i_compMul(navVector2i a, navVector2i b) { return navVector2i{ a.x * b.x, a.y * b.y }; }
navVector2i navVec2i_perpCw(navVector2i v)   { return navVector2i{ v.y, -v.x }; }
navVector2i navVec2i_perpCcw(navVector2i v)  { return navVector2i{ -v.y, v.x }; }
int   navVec2i_cmp(navVector2i a, navVector2i b)   { const int c = std::abs(a.x - b.x); const int d = std::abs(a.y - b.y); return c > d ? c : d; }
float navVec2i_angle(navVector2i a, navVector2i b) { const float c = (float)navVec2i_dot(a, b); const float d = navVec2i_len(a) * navVec2i_len(b); return std::acos(c / d); }


bool navCircle_contains(const navCircle* self, navVector2f p) {
    return nav::FloatCircle{ fromc(self->pos), self->radius }.contains(fromc(p));
}

navVector2f navTriangle_centroid(const navTriangle* self, const navVector2f* vertices) {
    return intoc(nav::Triangle{ self->A, self->B, self->C, self->weight }.centroid((nav::Vector2f*)vertices));
}

navVector2f navTriangle_circumcenter(const navTriangle* self, const navVector2f* vertices) {
    return intoc(nav::Triangle{ self->A, self->B, self->C, self->weight }.circumcenter((nav::Vector2f*)vertices));
}

bool navTriangle_contains(const navTriangle* self, const navVector2f* vertices, navVector2f p, bool with_corners) {
    return nav::Triangle{ self->A, self->B, self->C, self->weight }.contains((nav::Vector2f*)vertices, fromc(p), with_corners);
}

bool navTriangle_contains_with_error(const navTriangle* self, const navVector2f* vertices, navVector2f p, float error) {
    return nav::Triangle{ self->A, self->B, self->C, self->weight }.contains_with_error((nav::Vector2f*)vertices, fromc(p), error);
}


struct navMesh { nav::Mesh impl; };

navMesh* navMesh_createFromFile(const char* filename, float scale) {
    navMesh* result = (navMesh*)malloc(sizeof(navMesh));
    if (!result) { return NULL; }
    new(&result->impl) nav::Mesh(nav::Mesh::read_file(filename, scale));
    return result;
}

void navMesh_writeToFile(const navMesh* self, const char* filename, float scale) {
    self->impl.write_file(filename, scale);
}

void navMesh_free(const navMesh* self) {
    self->impl.~Mesh();
    free((navMesh*)self);
}


size_t navMesh_getTriangleIndex(const navMesh* self, navVector2f p, float error) {
    if (auto t = self->impl.get_triangle(fromc(p), error)) {
        return *t;
    } else {
        return SIZE_MAX;
    }
}

const navVertexChain* navMesh_findPath(const navMesh* self, navVector2f begin, navVector2f end) {
    const auto path = self->impl.pathfind(fromc(begin), fromc(end));
    if (path.empty()) { return NULL; }
    navVertexChain* result = (navVertexChain*)malloc(sizeof(navVertexChain) + path.size() * sizeof(navVector2f));
    if (!result) { return NULL; }
    result->loop = false;
    result->count = path.size();
    for (size_t i = 0; i < path.size(); i++) {
        result->points[i] = intoc(path[i]);
    }
    return result;
}


navPolygonArray* navMesh_clonePolygons(const navMesh* self) {
    navPolygonArray* result = (navPolygonArray*)malloc(sizeof(navPolygonArray) + self->impl.polygons.size() * sizeof(navVertexChain*));
    if (!result) { return NULL; }
    result->count = self->impl.polygons.size();
    for (size_t i = 0; i < result->count; i++) {
        navVertexChain* chain = (navVertexChain*)malloc(sizeof(navVertexChain) + self->impl.polygons[i].size() * sizeof(navVector2f*));
        if (!result) { return NULL; }
        chain->loop = true;
        chain->count = self->impl.polygons[i].size();
        for (size_t j = 0; j < chain->count; j++) {
            chain->points[chain->count - 1 - j].x = (float)self->impl.polygons[i][j].x * 0.1f;
            chain->points[chain->count - 1 - j].y = (float)self->impl.polygons[i][j].y * 0.1f;
        }
        result->polys[i] = chain;
    }
    return result;
}

void navMesh_freePolygons(const navPolygonArray* self) {
    for (size_t i = 0; i < self->count; i++) {
        free((navVertexChain*)self->polys[i]);
    }
    free((navPolygonArray*)self);
}


navMesh* navMesh_createFromGrid(
        const uint8_t* grid,
        size_t width,
        size_t height,
        size_t stride,
        size_t index,
        navMethod method,
        float epsilon)
{
    navMesh* result = (navMesh*)malloc(sizeof(navMesh));
    if (!result) { return NULL; }
    new(&result->impl) nav::Mesh(nav::generate_delauney(grid, width, height, stride, index, (nav::Method)method, epsilon));
    return result;
}

#ifdef __cplusplus
}
#endif

