#ifndef GENERATE_H
#define GENERATE_H
#include <SFML/Graphics.h>
#include <stdint.h>


typedef uint64_t AdjacencyRule;

AdjacencyRule AdjacencyRule_create(size_t* indices, size_t count);
bool AdjacencyRule_permits(AdjacencyRule self, size_t value);
void AdjacencyRule_setBorderIndex(size_t index);

void waveFunctionCollapse(int32_t* board, sfVector2u boardsize, const AdjacencyRule* rules);


#endif
