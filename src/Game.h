#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.h>


typedef struct World World;
typedef struct navPolygonArray navPolygonArray;

void Game_Init(const World* world);
void Game_Update();
void Game_Render(sfRenderWindow* window);
// void Game_Destroy();


#endif
