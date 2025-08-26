#include "Game.h"
#include "Entity.h"
#include "SFML/Graphics.h"
#include "clock.h"
#include <stdio.h>
#include <math.h>

static Entity player;

void GameInit() {
    player = (Entity) {
        (sfVector2f) {0,0},
        (sfVector2f) {0,0},
        (sfFloatRect) {4*16-5,4*16-5,10,10},
        70.0f,
        100000.0f,
        400.0f,
        (Cooldown) {sfClock_create(),0.5f},
    };
}

// decelerates player by 10% of current velocity
void deceleratePlayer(Entity* player) {
    if (fabsf(sfVec2f_len(player->velocity)) > 0.1) {
        setVelocity(player, sfVec2f_scale(player->velocity,0.95));
    } else {
        setVelocity(player, (sfVector2f) {0,0});
    }
}

void GameUpdate() {
    processKeyClicked();
    deceleratePlayer(&player);
    moveEntity(&player, sfVec2f_scale(player.velocity,Clock_deltaTime()));
}

void GameRender(sfRenderWindow* window) {
    sfRectangleShape* playerRect = sfRectangleShape_create();
    sfRenderStates renderState = sfRenderStates_default();
    sfRectangleShape_setPosition(playerRect, (sfVector2f) {player.rectBound.left, player.rectBound.top});
    sfRectangleShape_setSize(playerRect, (sfVector2f) {player.rectBound.width, player.rectBound.height});
    sfRectangleShape_setFillColor(playerRect, sfBlue);
    sfRenderWindow_drawRectangleShape(window, playerRect, &renderState);
}

void processKeyClicked() {
    float dt = Clock_deltaTime();
    if (sfKeyboard_isKeyPressed(sfKeyW)) {
        addVelocity(&player, (sfVector2f) {0, -player.acc*dt});
    }
    if (sfKeyboard_isKeyPressed(sfKeyS)) {
        addVelocity(&player, (sfVector2f) {0, player.acc*dt});
    }
    if (sfKeyboard_isKeyPressed(sfKeyA)) {
        addVelocity(&player, (sfVector2f) {-player.acc*dt, 0});
    }
    if (sfKeyboard_isKeyPressed(sfKeyD)) {
        addVelocity(&player, (sfVector2f) {player.acc*dt, 0});
    }
    if (sfKeyboard_isKeyPressed(sfKeySpace)) {
        startDash(&player);
    }
}
