#include "Game.h"
#include "Entity.h"
#include "SFML/Graphics.h"
#include "SFML/Graphics/RenderStates.h"
#include "SFML/Graphics/RenderWindow.h"
#include "SFML/Window/Keyboard.h"
#include "clock.h"

static Entity player;

void GameInit() {
    player = (Entity) {
        (sfVector2f) {0,0},
        (sfVector2f) {0,0},
        (sfFloatRect) {-50,-50,100,100},
        5.0f,
        25.0f,
        (Cooldown) {sfClock_create(),2.0f},
    };
}

void GameUpdate() {
    updateVelocity(&player);
}

void GameRender(sfRenderWindow* window) {
    sfRectangleShape* playerRect = sfRectangleShape_create();
    sfRenderStates renderState = sfRenderStates_default();
    sfRectangleShape_setPosition(playerRect, player.position);
    sfRectangleShape_setSize(playerRect, (sfVector2f) {player.rectBound.width, player.rectBound.height});
    sfRectangleShape_setFillColor(playerRect, sfBlue);
    sfRenderWindow_drawRectangleShape(window, playerRect, &renderState);
}

void processKeyClicked(sfEvent event) {
    float dt = Clock_deltaTime();
    float dtSq = dt*dt;
    if (sfKeyboard_isKeyPressed(sfKeyW)) {
        addVelocity(&player, (sfVector2f) {0, -player.speed*dtSq});
    }
    else if (sfKeyboard_isKeyPressed(sfKeyS)) {
        addVelocity(&player, (sfVector2f) {0, player.speed*dtSq});
    }
    else if (sfKeyboard_isKeyPressed(sfKeyA)) {
        addVelocity(&player, (sfVector2f) {-player.speed*dtSq, 0});
    }
    else if (sfKeyboard_isKeyPressed(sfKeyD)) {
        addVelocity(&player, (sfVector2f) {player.speed*dtSq, 0});
    }
    else if (sfKeyboard_isKeyPressed(sfKeySpace)) {
        startDash(&player);
    }
}
