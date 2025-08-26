#include "Game.h"
#include "Entity.h"
#include "SFML/Graphics.h"
#include "SFML/Window/Keyboard.h"
#include "clock.h"

static Entity player;

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
