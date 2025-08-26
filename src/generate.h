#ifndef GENERATE_H
#define GENERATE_H
#include <SFML/Graphics.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct AdjacencyRule {
    
    uint64_t rules[8];
} AdjacencyRule;

AdjacencyRule AdjacencyRule_create(size_t* indices, size_t count);
bool AdjacencyRule_permits(AdjacencyRule self, size_t value);
void AdjacencyRule_setBorderIndex(size_t index);

void waveFunctionCollapse(int32_t* board, sfVector2u boardsize, const AdjacencyRule* rules);


#endif
