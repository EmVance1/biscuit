#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.h>


typedef struct World World;
typedef struct navPolygonArray navPolygonArray;

void Game_Init(const World* _world);
void Game_Update(const sfRenderWindow* window, sfView* camera);
void Game_Render(sfRenderWindow* window);
// void Game_Destroy();


#endif
