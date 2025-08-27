#include <SFML/Graphics.h>
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include "clock.h"

static Entity player;


static void processKeyClicked();

void Game_Init() {
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
static void deceleratePlayer(Entity* _player) {
    if (fabsf(sfVec2f_len(_player->velocity)) > 0.1) {
        Entity_setVelocity(_player, sfVec2f_scale(_player->velocity, 0.95f));
    } else {
        Entity_setVelocity(_player, (sfVector2f) {0,0});
    }
}

void Game_Update() {
    processKeyClicked();
    deceleratePlayer(&player);
    Entity_move(&player, sfVec2f_scale(player.velocity,Clock_deltaTime()));
}

void Game_Render(sfRenderWindow* window) {
    sfRectangleShape* playerRect = sfRectangleShape_create();
    sfRenderStates renderState = sfRenderStates_default();
    sfRectangleShape_setPosition(playerRect, (sfVector2f) {player.rectBound.left, player.rectBound.top});
    sfRectangleShape_setSize(playerRect, (sfVector2f) {player.rectBound.width, player.rectBound.height});
    sfRectangleShape_setFillColor(playerRect, sfBlue);
    sfRenderWindow_drawRectangleShape(window, playerRect, &renderState);
}


static void processKeyClicked() {
    float dt = Clock_deltaTime();
    if (sfKeyboard_isKeyPressed(sfKeyW)) {
        Entity_addVelocity(&player, (sfVector2f) {0, -player.acc*dt});
    }
    if (sfKeyboard_isKeyPressed(sfKeyS)) {
        Entity_addVelocity(&player, (sfVector2f) {0, player.acc*dt});
    }
    if (sfKeyboard_isKeyPressed(sfKeyA)) {
        Entity_addVelocity(&player, (sfVector2f) {-player.acc*dt, 0});
    }
    if (sfKeyboard_isKeyPressed(sfKeyD)) {
        Entity_addVelocity(&player, (sfVector2f) {player.acc*dt, 0});
    }
    if (sfKeyboard_isKeyPressed(sfKeySpace)) {
        Entity_startDash(&player);
    }
}

