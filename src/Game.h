#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.h>


void Game_Init();
void Game_Update();
void Game_Render(sfRenderWindow* window);

void Game_Destroy();

void attackMelee();

void animateSword(sfRenderWindow* window);

void renderEnemies(sfRenderWindow* window);

void renderPlayer(sfRenderWindow* window);

#endif
