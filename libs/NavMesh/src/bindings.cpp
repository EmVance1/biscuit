#define NAVMESH_GENERATION
#include "navmesh/c/lib.h"
#include "navmesh/lib.h"


static nav::Vector2f fromc(navVector2f v) { return nav::Vector2f{ v.x, v.y }; }
static navVector2f intoc(nav::Vector2f v) { return navVector2f{ v.x, v.y }; }


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
    result->impl = nav::Mesh::read_file(filename, scale);
    return result;
}

void navMesh_writeToFile(const navMesh* self, const char* filename, float scale) {
    self->impl.write_file(filename, scale);
}

void navMesh_free(const navMesh* self) {
    free((navMesh*)self);
}


size_t navMesh_getTriangleIndex(const navMesh* self, navVector2f p, float error) {
    if (auto t = self->impl.get_triangle(fromc(p), error)) {
        return *t;
    } else {
        return SIZE_MAX;
    }
}

navPath* navMesh_findPath(const navMesh* self, navVector2f begin, navVector2f end) {
    const auto path = self->impl.pathfind(fromc(begin), fromc(end));
    navPath* result = (navPath*)malloc(sizeof(navPath) + path.size() * sizeof(navVector2f));
    result->count = path.size();
    for (size_t i = 0; i < path.size(); i++) {
        result->points[i] = intoc(path[i]);
    }
    return result;
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
    result->impl = nav::generate_delauney(grid, width, height, stride, index, (nav::Method)method, epsilon);
    return result;
}

