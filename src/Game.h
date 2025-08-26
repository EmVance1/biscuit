#ifndef GAME_H
#define GAME_H
#include "SFML/Graphics.h"

void processKeyClicked(sfEvent event);

void GameInit();

void GameUpdate();

void GameRender(sfRenderWindow* window);
#endif
