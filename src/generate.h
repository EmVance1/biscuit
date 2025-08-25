#ifndef GENERATE_H
#define GENERATE_H
#include <stdint.h>


typedef struct AdjacencyRule {
    size_t count;
    size_t permits[];
} AdjacencyRule;

AdjacencyRule* AdjacencyRule_create(size_t* indices, size_t count);
void AdjacencyRule_free(const AdjacencyRule* self);


#endif
