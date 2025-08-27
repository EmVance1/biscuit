#include <SFML/Graphics.h>
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include "clock.h"
#include <stdio.h>
#include <math.h>
#include <malloc.h>

#define PI 3.141592653589

static Entity player;
static Entity* enemies;
static int numEnemies;


static void processKeyClicked();

void Game_Init() {
    player = (Entity) {
        (sfVector2f) {4*16,4*16}, // position
        (sfVector2f) {0,0}, // velocity
        (sfVector2f) {1,0}, // lastDir
        (sfFloatRect) {4*16-5,4*16-5,10,10}, // rectBound
        90.0f, // speed
        10000.0f, // acc(eleration)
        600.0f, // dashSpeed
        100, // health
        20.0f, // meleeRange
        40.0f, // meleeDamage
        .dashCooldown = cooldownCreate(0.5f), // dashCooldown
        .attackCooldown = cooldownCreate(0.3f), // attackCooldown
        .attackAnim = cooldownCreate(0.2f),
    };

    numEnemies = 2;
    enemies = (Entity*) malloc(sizeof(Entity)*numEnemies);
    enemies[0] = (Entity) {
        (sfVector2f) {6*15,3*16}, // position
        (sfVector2f) {0,0}, // velocity
        (sfVector2f) {1,0}, // lastDir
        (sfFloatRect) {6*16-5,3*16-5,10,10}, // rectBound
        90.0f, // speed
        10000.0f, // acc(eleration)
        600.0f, // dashSpeed
        100, // health
        70.0f, // meleeRange
        20.0f, // meleeDamage
        .dashCooldown = cooldownCreate(0.5f), // dashCooldown
        .attackCooldown = cooldownCreate(1.0f), // attackCooldown
        .attackAnim = cooldownCreate(0.2f),
    };
    enemies[1] = (Entity) {
        (sfVector2f) {4*16,7*16}, // position
        (sfVector2f) {0,0}, // velocity
        (sfVector2f) {1,0}, // lastDir
        (sfFloatRect) {4*16-5,7*16-5,10,10}, // rectBound
        90.0f, // speed
        10000.0f, // acc(eleration)
        600.0f, // dashSpeed
        100, // health
        70.0f, // meleeRange
        20.0f, // meleeDamage
        .dashCooldown = cooldownCreate(0.5f), // dashCooldown
        .attackCooldown = cooldownCreate(1.0f), // attackCooldown
        .attackAnim = cooldownCreate(0.2f),
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

static float getCooldown(Cooldown cooldown) {
    return cooldown.cooldownLength - sfTime_asSeconds(sfClock_getElapsedTime(cooldown.clock));
}

void Game_Update() {
    processKeyClicked();
    deceleratePlayer(&player);
    Entity_move(&player);
}

void attackMelee() {
    if (getCooldown(player.attackCooldown) > 0)
        return;

    // start attack
    cooldownRestart(player.attackAnim);
    cooldownRestart(player.attackCooldown);
    player.attackStartAngle = 180.0f*atan2f(player.lastDir.y,player.lastDir.x)/PI;

    // find enemies in range
    for (int i=0; i<numEnemies; i++) {
        sfVector2f ab = sfVec2f_sub(player.position,enemies[i].position);
        if (sfVec2f_len(ab) >= player.meleeRange) {
            Entity_damage(&enemies[i],player.meleeDamage);
        }
    }
}

void Game_Render(sfRenderWindow* window) {
    renderPlayer(window);
    renderEnemies(window);
    animateSword(window);
}

float lerp(float a, float b, float t) {
    return a+(b-a)*t;
}

void renderEnemies(sfRenderWindow* window) {
    sfRectangleShape* rect = sfRectangleShape_create();
    sfRenderStates renderState = sfRenderStates_default();
    sfRectangleShape_setSize(rect, (sfVector2f) {player.rectBound.width, player.rectBound.height});
    for (int i=0; i<numEnemies; i++) {
        sfRectangleShape_setPosition(rect, (sfVector2f) {enemies[i].rectBound.left,enemies[i].rectBound.top});
        sfRectangleShape_setFillColor(rect, sfRed);
        sfRenderWindow_drawRectangleShape(window, rect, &renderState);
    }
}

void renderPlayer(sfRenderWindow* window) {
    sfRectangleShape* rect = sfRectangleShape_create();
    sfRenderStates renderState = sfRenderStates_default();
    sfRectangleShape_setPosition(rect, (sfVector2f) {player.rectBound.left, player.rectBound.top});
    sfRectangleShape_setSize(rect, (sfVector2f) {player.rectBound.width, player.rectBound.height});
    sfRectangleShape_setFillColor(rect, sfBlue);
    sfRenderWindow_drawRectangleShape(window, rect, &renderState);
}

void animateSword(sfRenderWindow* window) {
    sfRenderStates renderState = sfRenderStates_default();

    if (cooldownGet(player.attackAnim) >= 0) {
        float t = 1 - cooldownGet(player.attackAnim)/player.attackAnim.cooldownLength;
        float angle = lerp(player.attackStartAngle-90.0f, player.attackStartAngle+90.0f, t);
        sfRectangleShape* swordRect = sfRectangleShape_create();
        sfRectangleShape_setOrigin(swordRect, (sfVector2f) {0.0f,2.5f});
        sfRectangleShape_setPosition(swordRect, player.position);
        sfRectangleShape_setSize(swordRect, (sfVector2f) {player.meleeRange, 5.0f});
        sfRectangleShape_setRotation(swordRect, angle);
        sfRectangleShape_setFillColor(swordRect, sfMagenta);

        sfRenderWindow_drawRectangleShape(window, swordRect, &renderState);
    }
}

void Game_Destroy() {
    free(enemies);
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
    if (sfMouse_isButtonPressed(sfMouseLeft)) {
        attackMelee();
    }
}

