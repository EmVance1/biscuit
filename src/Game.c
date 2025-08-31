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
#include "utils/circle.h"
#include "utils/clock.h"
#include "gui.h"
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
static int entitiesToKill;

#define PROJECTILE_MAX 32
// new projectiles are added to the first free index
// projectile being freed frees the index
// adding a new projectile with PROJECTILE_MAX active projectiles is "undefined"
static Projectile projectiles[PROJECTILE_MAX];

static const World* world;
static sfTexture* swordTexture;
static Cooldown spawntimer;
static size_t totalspawned;
static size_t roomwavesize;

static int totalAbilities = 5;
static Ability abilityChoices[2];
static int abilitiesRemaining = 4;
static Ability abilities[5] = {true, true, true, true, true};

#define UNIMPLEMENTED() do { fprintf(stderr, "function at '%s' line %d not implemented", __FILE__, __LINE__); exit(1); } while (0)


static void nextLevel(int chosen);
static void spawnEnemy(void);
static float lerp(float a, float b, float t);
static void processKeyClicked(const sfRenderWindow* window, const sfView* camera);
static void animateSword(sfRenderWindow* window);
static void renderDoors(sfRenderWindow* window);
static void deceleratePlayer(void);
static void attackMelee(void);
static void dash(void);
static void stunArea(void);
static void castFireball(const sfRenderWindow* window, const sfView* camera);
static void castHazardCloud(const sfRenderWindow* window, const sfView* camera);
static void updateProjectiles(void);
static void effectProjectile(Projectile* projectile);
static int chooseFirstAbility(void);
static int chooseSecAbility(void);


void Game_Init(const World* _world) {
    world = _world;

    Entity_loadTextures();
    swordTexture = sfTexture_createFromFile("res/textures/sword.png", NULL);

    memset(entities, 0, ENTITY_MAX);
    for (int i=0; i<ENTITY_MAX; i++) {
        entities[i] = (Entity) {.is_alive = false} ;
    }
    entities[PLAYER_INDEX] = Entity_createPlayer((sfVector2f){ 4*16, 4*16 });
    entity_count = 1;
    totalspawned = 0;
    roomwavesize = 64;
    entitiesToKill = 10;
    player = &entities[PLAYER_INDEX];
    for (int i = 0; i < 4; i++) {
        spawnEnemy();
        totalspawned++;
    }
    spawntimer = Cooldown_create(1.0f);

    for (int i = 0; i < PROJECTILE_MAX; i++) {
        projectiles[i].is_alive = false;
    }

    abilityChoices[0] = chooseFirstAbility();
    abilityChoices[1] = chooseSecAbility();

    Gui_init(
        player->attackCooldown.cooldownLength,
        player->dashCooldown.cooldownLength,
        player->fireballCooldown.cooldownLength,
        player->hazardCooldown.cooldownLength
    );
}

void Game_Update(const sfRenderWindow* window, sfView* camera) {
    processKeyClicked(window, camera);

    for (int i=0; i<2; i++) {
        if (entitiesToKill > 0) break;
        if (Collision_PlayerRect(player, world->doors[i])) {
            player->abilities[abilityChoices[i]] = false;
            nextLevel(i);
        }
    }

    Collision_HandlePlayerNavmesh(player, world->colliders, world->mesh_to_world);
    for (int i = 0; i < ENTITY_MAX; i++) {
        if (entities[i].is_alive) {
            Entity_move(&entities[i], player, world->mesh_to_world);
        }
    }

    updateProjectiles();
    for (int i = 0; i < PROJECTILE_MAX; i++) {
        if (projectiles[i].is_alive && !projectiles[i].is_dying) {
            Projectile_move(&projectiles[i]);
        }
    }

    if (Cooldown_ready(&spawntimer) && entity_count < ENTITY_MAX && totalspawned < roomwavesize) {
        spawnEnemy();
        Cooldown_reset(&spawntimer);
        totalspawned++;
    }

    const sfVector2f center = sfView_getCenter(camera);
    const sfVector2f dir = sfVec2f_sub(player->position, center);
    sfView_move(camera, sfVec2f_scale(dir, 0.005f));

    const float hit_cooldown = Cooldown_get(&player->attackCooldown);
    const float dash_cooldown = Cooldown_get(&player->dashCooldown);
    const float fb_cooldown = Cooldown_get(&player->fireballCooldown);
    const float hz_cooldown = Cooldown_get(&player->hazardCooldown);
    Gui_update(
        hit_cooldown > 0.f ? hit_cooldown : INFINITY,
        dash_cooldown > 0.f ? dash_cooldown : INFINITY,
        fb_cooldown > 0.f ? fb_cooldown : INFINITY,
        hz_cooldown > 0.f ? hz_cooldown : INFINITY
    );
}

void Game_Render(sfRenderWindow* window) {
    for (int i = 0; i < ENTITY_MAX; i++) {
        if (entities[i].is_alive) {
            Entity_render(window, &entities[i]);
        }
    }
    for (int i = 0; i < PROJECTILE_MAX; i++) {
        if (projectiles[i].is_alive) {
            Projectile_render(window, &projectiles[i]);
        }
    }
    animateSword(window);
    renderDoors(window);
}


/*
Static functions
*/

static int chooseFirstAbility(void) {
    int first = rand() % abilitiesRemaining;
    printf("First choice: %d\n", first);
    for (int j=0; j<totalAbilities; j++) {
        if (!first) {
            return j;
        }
        first -= abilities[j] ? 1 : 0;
    }
    return -1;
}

static int chooseSecAbility(void) {
    int second = rand() % (abilitiesRemaining-1);
    printf("Second choice: %d\n", second);
    for (int j=0; j<totalAbilities; j++) {
        if (!second) {
            return j;
        }
        second -= abilities[j] || j == (int) abilityChoices[0] ? 1 : 0;
    }
    return -1;
}

static void nextLevel(int chosen) {
    if (abilitiesRemaining < 2) {
    } else {
        abilitiesRemaining--;
        abilities[abilityChoices[chosen]] = false;
        printf("Chosen Ability: %d\n", abilityChoices[chosen]);
        Game_Init(world);
        for (int i=0; i<totalAbilities; i++) {
            player->abilities[i] = abilities[i];
        }
    }
}

static void spawnEnemy(void) {
    if (entity_count == ENTITY_MAX) return;

    for (int i = 0; i < PLAYER_INDEX; i++) {
        if (!entities[i].is_alive) {
            navVector2f pos = (navVector2f){ (float)(rand() % (int)world->gridsize.x), (float)(rand() % (int)world->gridsize.y) };
            while (navMesh_getTriangleIndex(world->navmesh, pos, 0.1f) == SIZE_MAX) {
                pos = (navVector2f){ (float)(rand() % (int)world->gridsize.x), (float)(rand() % (int)world->gridsize.y) };
            }
            entities[i] = Entity_createEnemy((sfVector2f){ pos.x * world->mesh_to_world, pos.y * world->mesh_to_world }, world->navmesh, world->mesh_to_world);
            entity_count++;
            return;
        }
    }
}

static void doEnemyAI(void) {
}

static float lerp(float a, float b, float t) {
    return a+(b-a)*t;
}

static void processKeyClicked(const sfRenderWindow* window, const sfView* camera) {
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
        dash();
    }
    if (sfMouse_isButtonPressed(sfMouseLeft)) {
        attackMelee();
    }
    if (sfKeyboard_isKeyPressed(sfKeyF)) {
        castFireball(window, camera);
    }
    if (sfKeyboard_isKeyPressed(sfKeyH)) {
        castHazardCloud(window, camera);
    }
}

// decelerates player by 2% of current velocity
static void deceleratePlayer(void) {
    if (sfVec2f_lenSquared(player->velocity) > 0.01f) {
        Entity_setVelocity(player, sfVec2f_scale(player->velocity, powf(0.80f, Clock_deltaTime() * 60.f)));
    } else {
        Entity_setVelocity(player, (sfVector2f){ 0, 0 });
    }
}

static void attackMelee(void) {
    if (!Cooldown_ready(&player->attackCooldown) || !player->abilities[A_ATTACK]) return;

    // start attack
    Cooldown_reset(&player->attackAnim);
    Cooldown_reset(&player->attackCooldown);
    player->attackStartAngle = atan2f(player->lastDir.y, player->lastDir.x) * INTO_DEG;

    // find enemies in range
    for (int i = 0; i < PLAYER_INDEX; i++) {
        if (!entities[i].is_alive) continue;
        const sfVector2f ab = sfVec2f_sub(player->position, entities[i].position);
        const float range = player->meleeRange + (entities[i].rectBound.width/2.0f);
        if (sfVec2f_lenSquared(ab) <= range * range) {
            Entity_damage(&entities[i], player->meleeDamage);
            if (!entities[i].is_alive) {
                entitiesToKill--;
                entity_count--;
            }
        }
    }
}

static void dash(void) {
    Entity_startDash(player);
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
        sfRectangleShape_setTexture(swordRect, swordTexture, true);

        sfRenderWindow_drawRectangleShape(window, swordRect, &renderState);
    }
}

static void renderDoors(sfRenderWindow* window) {
    if (entitiesToKill > 0) return;
    sfRectangleShape* rect = sfRectangleShape_create();
    for (int i=0; i<2; i++) {
        sfVector2f position = (sfVector2f) {world->doors[i].left,world->doors[i].top};
        sfVector2f size = (sfVector2f) {world->doors[i].width,world->doors[i].height};
        sfRectangleShape_setPosition(rect, position);
        sfRectangleShape_setSize(rect, size);
        sfRectangleShape_setFillColor(rect, sfColor_fromRGB(100, 180, 30));
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }
}

static bool entityInRange(Entity* first, Entity* second, float range) {
    return sfVec2f_lenSquared(sfVec2f_sub(first->position, second->position)) <= range * range;
}

static void stunArea(void) {
    if (!Cooldown_ready(&player->stunCooldown) || !player->abilities[A_STUN]) return;

    Cooldown_reset(&player->stunCooldown);

    for (int i = 0; i < PLAYER_INDEX; i++) {
        if (entities[i].is_alive && entityInRange(player, &entities[i], player->stunRange)) {
            Entity_stun(&entities[i],player->stunDuration);
        }
    }
}

static void castFireball(const sfRenderWindow* window, const sfView* camera) {
    if (!Cooldown_ready(&player->fireballCooldown) || !player->abilities[A_FIREBALL]) return;

    Cooldown_reset(&player->fireballCooldown);

    for (int i=0; i<PROJECTILE_MAX; i++) {
        if (projectiles[i].is_alive) continue;
        float radius = 12;
        const sfVector2i mouse = sfMouse_getPositionRenderWindow(window);
        const sfVector2f direction = sfVec2f_norm(sfVec2f_sub(sfRenderWindow_mapPixelToCoords(window, mouse, camera), player->position));
        const sfVector2f position = sfVec2f_add(player->position, sfVec2f_scale(direction, 10));
        const sfVector2f velocity = sfVec2f_scale(direction, 10.f);
        projectiles[i] = Projectile_createFireball(position, velocity);
        break;
    }
}

static void castHazardCloud(const sfRenderWindow* window, const sfView* camera) {
    if (!Cooldown_ready(&player->hazardCooldown) || !player->abilities[A_HAZARD]) return;

    Cooldown_reset(&player->hazardCooldown);

    for (int i=0; i<PROJECTILE_MAX; i++) {
        if (projectiles[i].is_alive) continue;
        float radius = 12;
        const sfVector2i mouse = sfMouse_getPositionRenderWindow(window);
        const sfVector2f direction = sfVec2f_norm(sfVec2f_sub(sfRenderWindow_mapPixelToCoords(window, mouse, camera), player->position));
        const sfVector2f position = sfVec2f_add(player->position, sfVec2f_scale(direction, 10));
        projectiles[i] = Projectile_createHazard(position);
        Cooldown_reset(&projectiles[i].duration);
        break;
    }
}

static void effectProjectile(Projectile* projectile) {
    sfuCircle circle = (sfuCircle){ projectile->position, projectile->effectRadius };
    if (!Cooldown_ready(&projectile->dot)) return;
    Cooldown_reset(&projectile->dot);
    for(int j = 0; j < PLAYER_INDEX - 1; j++) {
        if (!entities[j].is_alive) continue;
        const sfFloatRect bound = entities[j].rectBound;
        if (sfuCircle_intersectsRect(circle, bound)) {
            Entity_damage(&entities[j], projectile->damage);
            if (!entities[j].is_alive) entitiesToKill--;
        }
    }
    // can't start killing the projectile here, because the projectile cloud remains for a while
    // Projectile_startKill(projectile);
}


static void updateProjectiles(void) {
    for (int i = 0; i < PROJECTILE_MAX; i++) {
        if (projectiles[i].is_dying) {
            if (Cooldown_ready(&projectiles[i].impactTimer)) {
                Projectile_kill(&projectiles[i]);
            }

        } else if (projectiles[i].is_alive) {
            if (projectiles[i].projType == HAZARD) {
                if (Cooldown_ready(&projectiles[i].duration)) {
                    Projectile_kill(&projectiles[i]);
                    return;
                } else {
                    effectProjectile(&projectiles[i]);
                    return;
                }
            } else if (Collision_ProjectileWallNavmesh(&projectiles[i], world->colliders, world->mesh_to_world)) {
                effectProjectile(&projectiles[i]);
                Projectile_startKill(&projectiles[i]);
                return;
            }
            const sfuCircle circle = (sfuCircle){ projectiles[i].position, projectiles[i].collisionRadius };
            for (int j = 0; j < PLAYER_INDEX; j++) {
                if (!entities[j].is_alive) continue;
                const sfFloatRect bound = entities[j].rectBound;
                if (sfuCircle_intersectsRect(circle, bound)) {
                    effectProjectile(&projectiles[i]);
                    Projectile_startKill(&projectiles[i]);
                    return;
                }
            }
        }
    }
}
