#include <SFML/Graphics.h>
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include "PlayerCollision.h"
#include "clock.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <malloc.h>
#include "navmesh/c/mesh.h"

#define PI 3.141592653589

static Entity player;
static Entity* enemies;
static int numEnemies;
static navPolygonArray* meshPolygons;


static float lerp(float a, float b, float t);
static void processKeyClicked();
static void animateSword(sfRenderWindow* window);
static void deceleratePlayer(Entity* _player);
static void handleWallCollisions();
static void attackMelee();
static void stunArea();
static void castFireball();
static void caseHazardCloud();


void Game_Init() {
    player = (Entity) {
        .position = (sfVector2f) {4*16,4*16},
        .velocity = (sfVector2f) {0,0},
        .lastDir = (sfVector2f) {1,0},
        .rectBound = (sfFloatRect) {4*16-5,4*16-5,10,10},
        .fillCol = sfBlue,
        .speed = 90.0f,
        .acc = 10000.0f,
        .dashSpeed = 600.0f,
        .health = 100,
        .meleeRange = 25.0f,
        .meleeDamage = 40.0f,
        .dashCooldown = Cooldown_Create(0.5f),
        .attackCooldown = Cooldown_Create(0.3f),
        .attackAnim = Cooldown_Create(0.2f),
        .damageAnim = Cooldown_Create(0.05f),
    };

    numEnemies = 2;
    enemies = (Entity*) malloc(sizeof(Entity)*numEnemies);
    enemies[0] = (Entity) {
        .position = (sfVector2f) {6*15,3*16},
        .velocity = (sfVector2f) {0,0},
        .lastDir = (sfVector2f) {1,0},
        .rectBound = (sfFloatRect) {6*16-5,3*16-5,10,10},
        .fillCol = sfColor_fromRGBA(255,165,0,255),
        .speed = 90.0f,
        .acc = 10000.0f,
        .dashSpeed = 600.0f,
        .health = 100,
        .meleeRange = 70.0f,
        .meleeDamage = 20.0f,
        .dashCooldown = Cooldown_Create(0.5f),
        .attackCooldown = Cooldown_Create(1.0f),
        .attackAnim = Cooldown_Create(0.2f),
        .damageAnim = Cooldown_Create(0.05f),
    };
    enemies[1] = (Entity) {
        .position= (sfVector2f) {4*16,7*16},
        .velocity = (sfVector2f) {0,0},
        .lastDir = (sfVector2f) {1,0},
        .rectBound = (sfFloatRect) {4*16-5,7*16-5,10,10},
        .fillCol = sfColor_fromRGBA(255,165,0,255),
        .speed = 90.0f,
        .acc = 10000.0f,
        .dashSpeed = 600.0f,
        .health = 100,
        .meleeRange = 70.0f,
        .meleeDamage = 20.0f,
        .dashCooldown = Cooldown_Create(0.5f),
        .attackCooldown = Cooldown_Create(1.0f),
        .attackAnim = Cooldown_Create(0.2f),
        .damageAnim = Cooldown_Create(0.05f),
    };
}


void Game_SetMeshPolygons(navPolygonArray* meshPoly) {
    meshPolygons = meshPoly;
}


void Game_Update() {
    processKeyClicked();

    Collision_HandlePlayerNavmesh(&player, meshPolygons);
    printf("Collisions done");
    deceleratePlayer(&player);
    Entity_move(&player);
}


void Game_Render(sfRenderWindow* window) {
    animateSword(window);
    Entity_render(window, &player);
    for (int i=0; i<numEnemies; i++) {
        Entity_render(window, &enemies[i]);
    }
}


void Game_Destroy() {
    free(enemies);
}


/*
 * Static functions
*/

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


// decelerates player by 10% of current velocity
static void deceleratePlayer(Entity* _player) {
    if (fabsf(sfVec2f_len(_player->velocity)) > 0.1) {
        Entity_setVelocity(_player, sfVec2f_scale(_player->velocity, 0.95f));
    } else {
        Entity_setVelocity(_player, (sfVector2f) {0,0});
    }
}


static void attackMelee() {
    if (Cooldown_Get(player.attackCooldown) > 0)
        return;

    // start attack
    Cooldown_Reset(player.attackAnim);
    Cooldown_Reset(player.attackCooldown);
    player.attackStartAngle = 180.0f*atan2f(player.lastDir.y,player.lastDir.x)/PI;

    // find enemies in range
    for (int i=0; i<numEnemies; i++) {
        sfVector2f ab = sfVec2f_sub(player.position,enemies[i].position);
        if (sfVec2f_len(ab) <= player.meleeRange+enemies[i].rectBound.width/2.0f) {
            Entity_damage(&enemies[i],player.meleeDamage);
        }
    }
}


static float lerp(float a, float b, float t) {
    return a+(b-a)*t;
}


static void animateSword(sfRenderWindow* window) {
    sfRenderStates renderState = sfRenderStates_default();

    if (Cooldown_Get(player.attackAnim) >= 0) {
        float t = 1 - Cooldown_Get(player.attackAnim)/player.attackAnim.cooldownLength;
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

static bool entityInRange(Entity* first, Entity* second, float range) {
   return sfVec2f_len(sfVec2f_sub(first->position,second->position)) <= range;
}


static void stunArea() {
   for (int i=0; i<numEnemies; i++) {
       if (entityInRange(&player, &enemies[i], player.stunRange)) {
           Cooldown_Set(enemies[i].stunEffect,player.stunDuration);
       }
   }
}
