#include "generate.h"
#include <stdlib.h>


AdjacencyRule* AdjacencyRule_create(size_t* indices, size_t count) {
    AdjacencyRule* rule = (AdjacencyRule*)malloc(sizeof(AdjacencyRule) + count * sizeof(size_t));
    if (!rule) { return NULL; }
    rule->count = count;
    for (size_t i = 0; i < count; i++) {
        rule->permits[i] = indices[i];
    }
    return rule;
}

void AdjacencyRule_free(const AdjacencyRule* self) {
    free((AdjacencyRule*)self);
}

