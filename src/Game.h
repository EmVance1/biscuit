#ifndef GAME_H
#define GAME_H
#include "SFML/Graphics.h"

void processKeyClicked();

void GameInit();

void GameUpdate();

void GameRender(sfRenderWindow* window);
#endif
