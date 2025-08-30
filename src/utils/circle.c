#include "circle.h"


bool sfuCircle_contains(sfuCircle self, sfVector2f point) {
    const float dist = sfVec2f_lenSquared(sfVec2f_sub(point, self.position));
    return dist <= self.radius * self.radius;
}

bool sfuCircle_intersectsCircle(sfuCircle self, sfuCircle other) {
    const float dist = sfVec2f_lenSquared(sfVec2f_sub(other.position, self.position));
    return dist <= (self.radius + other.radius) * (self.radius + other.radius);
}

bool sfuCircle_intersectsRect(sfuCircle self, sfFloatRect other) {
    const sfVector2f a = (sfVector2f){ other.left, other.top };
    const sfVector2f b = (sfVector2f){ other.left + other.width, other.top };
    const sfVector2f c = (sfVector2f){ other.left + other.width, other.top + other.height };
    const sfVector2f d = (sfVector2f){ other.left, other.top + other.height };
    const sfFloatRect r1 = (sfFloatRect){ other.left - self.radius, other.height, other.width + 2 * self.radius, other.height };
    const sfFloatRect r2 = (sfFloatRect){ other.left, other.top - self.radius,    other.width, other.height + 2 * self.radius };
    return sfFloatRect_contains(&r1, self.position.x, self.position.y) ||
           sfFloatRect_contains(&r2, self.position.x, self.position.y) ||
           (sfuCircle_contains(self, a) || sfuCircle_contains(self, b) || sfuCircle_contains(self, c) || sfuCircle_contains(self, d));
}

