#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.h>
#include "navmesh/c/mesh.h"


void Game_Init();
void Game_Update();
void Game_Render(sfRenderWindow* window);
void Game_Destroy();

void Game_SetMeshPolygons(navPolygonArray* meshPolygons);
#endif
