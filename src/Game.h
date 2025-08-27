#ifndef GAME_H
#define GAME_H
#include "SFML/Graphics.h"

void processKeyClicked();

void GameInit();

void GameUpdate();

void GameRender(sfRenderWindow* window);

void GameDestroy();

void attackMelee();

void animateSword(sfRenderWindow* window);

void renderEnemies(sfRenderWindow* window);

void renderPlayer(sfRenderWindow* window);

#endif
