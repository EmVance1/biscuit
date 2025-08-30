#include "gui.h"
#include "SFML/Graphics/Text.h"
#include <stdbool.h>
#include <math.h>


static float hit_max;
static float dash_max;
static float fireball_max;
static float hit_current;
static float dash_current;
static float fireball_current;

static sfTexture* hit_tex;
static sfTexture* dash_tex;
static sfTexture* fireball_tex;

static sfTexture* hit_but;
static sfTexture* dash_but;
static sfTexture* fireball_but;


void Gui_init(float _hit_max, float _dash_max, float _fireball_max) {
    hit_max = _hit_max;
    dash_max = _dash_max;
    fireball_max = _fireball_max;
    hit_current = 0;
    dash_current = 0;
    fireball_current = 0;

    hit_tex = sfTexture_createFromFile("res/textures/sword_ui.png", NULL);
    dash_tex = sfTexture_createFromFile("res/textures/dash_ui.png", NULL);
    fireball_tex = sfTexture_createFromFile("res/textures/fire_ui.png", NULL);

    hit_but = sfTexture_createFromFile("res/textures/hit_button.png", NULL);
    dash_but = sfTexture_createFromFile("res/textures/dash_button.png", NULL);
    fireball_but = sfTexture_createFromFile("res/textures/fireball_button.png", NULL);
}

void Gui_update(float _hit_current, float _dash_current, float _fireball_current) {
    hit_current = _hit_current;
    dash_current = _dash_current;
    fireball_current = _fireball_current;
}

void Gui_render(sfRenderWindow* window) {
    sfRectangleShape* rect = sfRectangleShape_create();

    sfRectangleShape_setPosition(rect, (sfVector2f){ 50, 50 });
    sfRectangleShape_setSize(rect, (sfVector2f){ 100, 100 });
    sfRectangleShape_setFillColor(rect, (sfColor){ 255, 255, 255, 255 });
    sfRectangleShape_setOutlineThickness(rect, 5.f);
    sfRectangleShape_setOutlineColor(rect, (sfColor){ 0, 0, 0, 255 });
    sfRectangleShape_setTexture(rect, hit_tex, true);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){ 200, 50 });
    sfRectangleShape_setTexture(rect, dash_tex, true);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){ 350, 50 });
    sfRectangleShape_setTexture(rect, fireball_tex, true);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setOutlineThickness(rect, 0.f);

    sfRectangleShape_setTexture(rect, NULL, true);

    sfRectangleShape_setFillColor(rect, (sfColor){ 255, 0, 0, 100 });
    if (hit_current < INFINITY) {
        const float height = 100 * (hit_current / hit_max);
        sfRectangleShape_setPosition(rect, (sfVector2f){ 50, 50 + 100 - height });
        sfRectangleShape_setSize(rect, (sfVector2f){ 100, height });
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }

    if (dash_current < INFINITY) {
        const float height = 100 * (dash_current / dash_max);
        sfRectangleShape_setPosition(rect, (sfVector2f){ 200, 50 + 100 - height });
        sfRectangleShape_setSize(rect, (sfVector2f){ 100, height });
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }

    if (fireball_current < INFINITY) {
        const float height = 100 * (fireball_current / fireball_max);
        sfRectangleShape_setPosition(rect, (sfVector2f){ 350, 50 + 100 - height });
        sfRectangleShape_setSize(rect, (sfVector2f){ 100, height });
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }

    sfRectangleShape_setPosition(rect, (sfVector2f){ 50, 160 });
    sfRectangleShape_setSize(rect, (sfVector2f){ 100, 100 });
    sfRectangleShape_setFillColor(rect, (sfColor){ 255, 255, 255, 255 });
    // sfRectangleShape_setOutlineThickness(rect, 5.f);
    // sfRectangleShape_setOutlineColor(rect, (sfColor){ 0, 0, 0, 255 });
    sfRectangleShape_setTexture(rect, hit_but, true);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){ 200, 160 });
    sfRectangleShape_setTexture(rect, dash_but, true);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){ 350, 160 });
    sfRectangleShape_setTexture(rect, fireball_but, true);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setOutlineThickness(rect, 0.f);
}

