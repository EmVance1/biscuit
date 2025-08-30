#include "gui.h"
#include <stdbool.h>
#include <math.h>


#define ATTACK 0
#define DASH 1
#define FIREBALL 2
#define HAZARD 3

static float maximum[4];
static float current[4];

static sfTexture* textures[4];
static sfTexture* buttons[4];


void Gui_init(float attack_max, float dash_max, float fireball_max, float hazard_max) {
    maximum[0] = attack_max;
    maximum[1] = dash_max;
    maximum[2] = fireball_max;
    maximum[3] = hazard_max;
    current[0] = 0;
    current[1] = 0;
    current[2] = 0;
    current[3] = 0;

    textures[0] = sfTexture_createFromFile("res/textures/attack_ui.png", NULL);
    textures[1] = sfTexture_createFromFile("res/textures/dash_ui.png", NULL);
    textures[2] = sfTexture_createFromFile("res/textures/fireball_ui.png", NULL);
    // textures[3] = sfTexture_createFromFile("res/textures/hazard_ui.png", NULL);

    buttons[0] = sfTexture_createFromFile("res/textures/attack_button.png", NULL);
    buttons[1] = sfTexture_createFromFile("res/textures/dash_button.png", NULL);
    buttons[2] = sfTexture_createFromFile("res/textures/fireball_button.png", NULL);
    buttons[3] = sfTexture_createFromFile("res/textures/hazard_button.png", NULL);
}

void Gui_update(float attack_current, float dash_current, float fireball_current, float hazard_current) {
    current[0] = attack_current;
    current[1] = dash_current;
    current[2] = fireball_current;
    current[3] = hazard_current;
}

void Gui_render(sfRenderWindow* window) {
    sfRectangleShape* rect = sfRectangleShape_create();

    sfRectangleShape_setSize(rect, (sfVector2f){ 100, 100 });
    sfRectangleShape_setFillColor(rect, (sfColor){ 255, 255, 255, 255 });
    sfRectangleShape_setOutlineThickness(rect, 5.f);
    sfRectangleShape_setOutlineColor(rect, (sfColor){ 0, 0, 0, 255 });
    for (int i = 0; i < 3; i++) {
        sfRectangleShape_setPosition(rect, (sfVector2f){ 50 + 150 * (float)i, 50 });
        sfRectangleShape_setTexture(rect, textures[i], true);
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }

    sfRectangleShape_setOutlineThickness(rect, 0.f);
    for (int i = 0; i < 4; i++) {
        sfRectangleShape_setPosition(rect, (sfVector2f){ 50 + 150 * (float)i, 160 });
        sfRectangleShape_setTexture(rect, buttons[i], true);
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }

    sfRectangleShape_setTexture(rect, NULL, true);
    sfRectangleShape_setFillColor(rect, (sfColor){ 255, 0, 0, 100 });
    for (int i = 0; i < 4; i++) {
        if (current[i] < INFINITY) {
            const float height = 100 * (current[i] / maximum[i]);
            sfRectangleShape_setPosition(rect, (sfVector2f){ 50 + 150 * (float)i, 50 + 100 - height });
            sfRectangleShape_setSize(rect, (sfVector2f){ 100, height });
            sfRenderWindow_drawRectangleShape(window, rect, NULL);
        }
    }

}

