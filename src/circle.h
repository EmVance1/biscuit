#ifndef SFU_CIRCLE_H
#define SFU_CIRCLE_H
#include <SFML/Graphics.h>
#include <stdbool.h>


typedef struct sfuCircle {
    sfVector2f position;
    float radius;
} sfuCircle;


bool sfuCircle_contains(sfuCircle self, sfVector2f point);
bool sfuCircle_intersectsCircle(sfuCircle self, sfuCircle other);
bool sfuCircle_intersectsRect(sfuCircle self, sfFloatRect other);


#endif
