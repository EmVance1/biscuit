#include <SFML/Graphics.h>
#include <navmesh/c/lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Game.h"
#include "Entity.h"
#include "PlayerCollision.h"
#include "SFML/System/Vector2.h"
#include "circle.h"
#include "clock.h"
#include "navmesh/c/shapes.h"
#include "world.h"


#define PI 3.141592f
#define INTO_DEG 180.f / PI
#define INTO_RAD PI / 180.f
#define INV_ROOT2 1.f / 1.414213f

#define ENTITY_MAX 128 + 1
#define PLAYER_INDEX ENTITY_MAX - 1
static Entity entities[ENTITY_MAX];
static Entity* player;
static size_t entity_count;

#define PROJECTILE_MAX 32
// new projectiles are added to the first free index
// projectile being freed frees the index
// adding a new projectile with PROJECTILE_MAX active projectiles is "undefined"
static Projectile projectiles[PROJECTILE_MAX];

static const World* world;
static sfTexture* swordTexture;

#define UNIMPLEMENTED() do { fprintf(stderr, "function at '%s' line %d not implemented", __FILE__, __LINE__); exit(1); } while (0)


static void spawnEnemy(void);
static float lerp(float a, float b, float t);
static void processKeyClicked(void);
static void animateSword(sfRenderWindow* window);
static void deceleratePlayer(void);
static void attackMelee(void);
static void stunArea(void);
static void castFireball(void);
static void caseHazardCloud(void);
static void updateProjectiles(void);


void Game_Init(const World* _world) {
    world = _world;

    Entity_loadTextures();
    swordTexture = sfTexture_createFromFile("res/textures/sword.png", NULL);

    memset(entities, 0, ENTITY_MAX);
    entity_count = 3;
    entities[PLAYER_INDEX] = Entity_createPlayer((sfVector2f){ 4*16, 4*16 });
    // entities[0] = Entity_createEnemy((sfVector2f){ 6*16, 3*16 }, (sfColor){ 255, 165, 0, 255 }, world->navmesh);
    // entities[1] = Entity_createEnemy((sfVector2f){ 4*16, 7*16 }, (sfColor){ 255, 165, 0, 255 }, world->navmesh);
    spawnEnemy();
    spawnEnemy();
    player = &entities[PLAYER_INDEX];

    for (int i = 0; i < PROJECTILE_MAX; i++) {
        projectiles[i] = Projectile_free();
    }

}

void Game_Update(sfView* camera) {
    processKeyClicked();
    Collision_HandlePlayerNavmesh(player, world->colliders, world->mesh_to_world);
    Entity_move(player);
    updateProjectiles();
    for (int i=0; i<PROJECTILE_MAX; i++) {
        if (!projectiles[i].free) {
            Projectile_move(&projectiles[i]);
        }
    }
    const sfVector2f center = sfView_getCenter(camera);
    const sfVector2f dir = sfVec2f_sub(player->position, center);
    sfView_move(camera, sfVec2f_scale(dir, 0.005f));
}

void Game_Render(sfRenderWindow* window) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        if (entities[i].is_alive) {
            Entity_render(window, &entities[i]);
        }
    }
    for (int i=0; i<PROJECTILE_MAX; i++) {
        if (!projectiles[i].free) {
            Projectile_render(window, &projectiles[i]);
        }
    }
    animateSword(window);
}


/*
Static functions
*/

static void spawnEnemy(void) {
    if (entity_count == ENTITY_MAX) return;

    for (int i = 0; i < PLAYER_INDEX; i++) {
        if (!entities[i].is_alive) {
            navVector2f pos = (navVector2f){ (float)(rand() % (int)world->gridsize.x), (float)(rand() % (int)world->gridsize.y) };
            while (navMesh_getTriangleIndex(world->navmesh, pos, 0.1f) == SIZE_MAX) {
                pos = (navVector2f){ (float)(rand() % (int)world->gridsize.x), (float)(rand() % (int)world->gridsize.y) };
            }
            entities[i] = Entity_createEnemy((sfVector2f){ pos.x * world->mesh_to_world, pos.y * world->mesh_to_world }, world->navmesh);
            entity_count++;
            return;
        }
    }
}

static float lerp(float a, float b, float t) {
    return a+(b-a)*t;
}

static void processKeyClicked(void) {
    sfVector2f movement = (sfVector2f){};
    if (sfKeyboard_isKeyPressed(sfKeyW)) {
        movement.y -= 1.f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyS)) {
        movement.y += 1.f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyA)) {
        movement.x -= 1.f;
    }
    if (sfKeyboard_isKeyPressed(sfKeyD)) {
        movement.x += 1.f;
    }
    if (sfVec2f_lenSquared(movement) > 1.1f) {
        movement.x *= INV_ROOT2;
        movement.y *= INV_ROOT2;
    }
    Entity_addVelocity(player, sfVec2f_scale(movement, player->acc * Clock_deltaTime() * 60.f));
    if (sfVec2f_lenSquared(movement) < 0.1f || !Cooldown_ready(&player->dashCooldown)) {
        deceleratePlayer();
    }

    if (sfKeyboard_isKeyPressed(sfKeySpace)) {
        Entity_startDash(player);
    }
    if (sfMouse_isButtonPressed(sfMouseLeft)) {
        attackMelee();
    }
    if (sfKeyboard_isKeyPressed(sfKeyF)) {
        castFireball();
    }
}

// decelerates player by 2% of current velocity
static void deceleratePlayer(void) {
    if (sfVec2f_lenSquared(player->velocity) > 0.01f) {
        Entity_setVelocity(player, sfVec2f_scale(player->velocity, powf(0.75f, Clock_deltaTime() * 60.f)));
    } else {
        Entity_setVelocity(player, (sfVector2f){ 0, 0 });
    }
}

static void attackMelee(void) {
    if (!Cooldown_ready(&player->attackCooldown)) return;

    // start attack
    Cooldown_reset(&player->attackAnim);
    Cooldown_reset(&player->attackCooldown);
    player->attackStartAngle = atan2f(player->lastDir.y, player->lastDir.x) * INTO_DEG;

    // find enemies in range
    for (int i = 0; i < PLAYER_INDEX; i++) {
        if (entities[i].is_alive) {
            const sfVector2f ab = sfVec2f_sub(player->position, entities[i].position);
            const float range = player->meleeRange + (entities[i].rectBound.width/2.0f);
            if (sfVec2f_lenSquared(ab) <= range * range) {
                Entity_damage(&entities[i], player->meleeDamage);
                if (!entities[i].is_alive) {
                    entity_count--;
                }
            }
        }
    }
}

static void animateSword(sfRenderWindow* window) {
    sfRenderStates renderState = sfRenderStates_default();

    if (Cooldown_get(&player->attackAnim) >= 0) {
        float t = 1 - Cooldown_get(&player->attackAnim) / player->attackAnim.cooldownLength;
        float angle = lerp(player->attackStartAngle-90.0f, player->attackStartAngle+90.0f, t);
        sfRectangleShape* swordRect = sfRectangleShape_create();
        sfRectangleShape_setOrigin(swordRect, (sfVector2f){ 0.0f, 10.f });
        sfRectangleShape_setPosition(swordRect, player->position);
        sfRectangleShape_setSize(swordRect, (sfVector2f){ player->meleeRange, 20.0f });
        sfRectangleShape_setRotation(swordRect, angle);
        // sfRectangleShape_setFillColor(swordRect, sfMagenta);
        sfRectangleShape_setTexture(swordRect, swordTexture, true);

        sfRenderWindow_drawRectangleShape(window, swordRect, &renderState);
    }
}

static bool entityInRange(Entity* first, Entity* second, float range) {
    return sfVec2f_lenSquared(sfVec2f_sub(first->position, second->position)) <= range * range;
}

static void stunArea(void) {
    if (!Cooldown_ready(&player->stunCooldown)) return;

    Cooldown_reset(&player->stunCooldown);

    for (int i = 0; i < PLAYER_INDEX; i++) {
        if (entities[i].is_alive && entityInRange(player, &entities[i], player->stunRange)) {
            Entity_stun(&entities[i],player->stunDuration);
        }
    }
}

static void castFireball(void) {
    // should throw fireball in direction of (latest) movement
    // move fireball position a bit every frame
    // when within radius of another entity, explode and deal larger radius of damage with falloff
    if (!Cooldown_ready(&player->fireballCooldown)) return;

    Cooldown_reset(&player->fireballCooldown);

    for (int i = 0; i < PROJECTILE_MAX; i++) {
       if (projectiles[i].free) {
            sfVector2f position = sfVec2f_add(player->position,sfVec2f_scale(player->lastDir,10));
            position = sfVec2f_add(position, (sfVector2f){ -player->rectBound.width/2, -player->rectBound.height/2 });
            sfVector2f velocity = sfVec2f_scale(player->lastDir, 6.f);
            projectiles[i] = Projectile_createFireball(position, velocity, 12, 40);
            break;
        }
    }
}

static void caseHazardCloud(void) {
    UNIMPLEMENTED();
}

static void effectProjectile(Projectile* projectile) {
    const sfuCircle circle = (sfuCircle){ projectile->position, projectile->effectRadius };
    for (int j = 0; j < PLAYER_INDEX; j++) {
        if (!entities[j].is_alive) continue;
        const sfFloatRect bound = entities[j].rectBound;
        if (sfuCircle_intersectsRect(circle, bound)) {
            switch (projectile->projType) {
                case FIREBALL:
                    Entity_damage(&entities[j], projectile->damage);
                    break;
                default:
                    break;
            }
        }
    }
    *projectile = Projectile_free();
}


static void updateProjectiles(void) {
    for (int i = 0; i < PROJECTILE_MAX; i++) {
        if (!projectiles[i].free) {
            if (Collision_ProjectileWallNavmesh(&projectiles[i], world->colliders, world->mesh_to_world)) {
                effectProjectile(&projectiles[i]);
                return;
            }
            const sfuCircle circle = (sfuCircle){ projectiles[i].position, projectiles[i].collisionRadius };
            for (int j = 0; j < PLAYER_INDEX; j++) {
                if (!entities[j].is_alive) continue;
                const sfFloatRect bound = entities[j].rectBound;
                if (sfuCircle_intersectsRect(circle, bound)) {
                    effectProjectile(&projectiles[i]);
                }

                /*
                const sfFloatRect bound = entities[i].rectBound;
                const sfVector2f v0 = (sfVector2f){ bound.left,             bound.top              };
                const sfVector2f v1 = (sfVector2f){ bound.left,             bound.top+bound.height };
                const sfVector2f v2 = (sfVector2f){ bound.left+bound.width, bound.top              };
                const sfVector2f v3 = (sfVector2f){ bound.left+bound.width, bound.top+bound.height };
                const sfVector2f v[4] = {v0,v1,v2,v3};
                if (Collision_ProjectileWall(&projectiles[i], v, 4, true)) {
                    effectProjectile(&projectiles[i],i);
                    return;
                }
                */
            }
        }
    }
}
