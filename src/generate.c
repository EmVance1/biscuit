#include "generate.h"
#include <string.h>
#include <stdlib.h>


AdjacencyRule AdjacencyRule_create(size_t* indices, size_t count) {
    AdjacencyRule rule = 0;
    for (size_t i = 0; i < count; i++) {
        rule |= 1 << (uint64_t)indices[i];
    }
    return rule;
}

bool AdjacencyRule_permits(AdjacencyRule self, size_t value) {
    return (self & (1 << value)) > 0;
}

/*
static int AdjacencyRule_count(const AdjacencyRule* rule) {
    int count = 0;
    for (size_t i = 0; i < sizeof(int64_t); i++) {
        count += rule->permits & (1 << i);
    }
    return count;
}
*/

static size_t s_borderIndex = 0;

void AdjacencyRule_setBorderIndex(size_t index) {
    s_borderIndex = index;
}


static int Set_size(uint64_t set) {
    int count = 0;
    for (size_t i = 0; i < sizeof(int64_t); i++) {
        count += set & (1 << i);
    }
    return count;
}

static uint64_t Set_intersection(uint64_t lhs, uint64_t rhs) {
    return lhs & rhs;
}

static uint64_t Set_union(uint64_t lhs, uint64_t rhs) {
    return lhs | rhs;
}


static int32_t getBoard(int32_t* board, sfVector2u size, sfVector2i pos) {
    if ((pos.x < 0 || pos.x >= size.x) || (pos.y < 0 || pos.y >= size.y)) {
        return (int32_t)s_borderIndex;
    }
    return board[pos.y * size.x + pos.x];
}

static void setBoard(int32_t* board, sfVector2u size, sfVector2i pos, int32_t value) {
    if ((pos.x < 0 || pos.x >= size.x) || (pos.y < 0 || pos.y >= size.y)) {
        return;
    }
    board[pos.y * size.x + pos.x] = value;
}

void waveFunctionCollapse(int32_t* board, sfVector2u boardsize, const AdjacencyRule* rules) {
    while (true) {
        // propagate collapse
        int64_t* possibilities = (int64_t*)malloc(sizeof(int64_t) * boardsize.x * boardsize.y);
        for (int y = 0; y < (int)boardsize.y; y++) {
            for (int x = 0; x < (int)boardsize.x; x++) {
                size_t v = getBoard(board, boardsize, (sfVector2i){ (int)x, (int)y });
                if (v >= 0) {
                    possibilities[y * boardsize.x + x] = 1 << v;
                } else {
                    uint64_t set = UINT64_MAX;
                    for (int j = -1; j < 2; j++) {
                        for (int i = -1; i < 2; i++) {
                            if (i == 0 && j == 0) { continue; }
                            const size_t neighbor = getBoard(board, boardsize, (sfVector2i){ (int)x+i, (int)y+j });
                            if (neighbor < 0) {
                                continue;
                            } else {
                                // set = Set_intersection(set, rules(v, neighbor));
                                set = Set_intersection(set, rules[neighbor]);
                            }
                        }
                    }
                    possibilities[y * boardsize.x + x] = set;
                }
            }
        }

        // find smallest set
        int min = 65;
        size_t min_x = SIZE_MAX;
        size_t min_y = SIZE_MAX;
        for (size_t y = 0; y < (size_t)boardsize.y; y++) {
            for (size_t x = 0; x < (size_t)boardsize.x; x++) {
                const int s = Set_size(possibilities[y * boardsize.x + x]);
                if (s == 1) { continue; }
                if (s < min) {
                    min = s;
                    min_x = x;
                    min_y = y;
                }
            }
        }

        // detect ending
        if (min == 65) {
            return;
        }

        // collapse smallest set
        bool choices[64] = { 0 };
        board[min_y * boardsize.x + min_x] = min;
    }
}

