#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.h>


void Game_Init(void);
void Game_Update(const sfRenderWindow* window, sfView* camera);
void Game_Render(sfRenderWindow* window);
// void Game_Destroy();


#endif
