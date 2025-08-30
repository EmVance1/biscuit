#include "gui.h"
#include "SFML/Graphics/RenderWindow.h"


static float fireball_max;
static float fireball_current;


void Gui_init(float _fireball_max) {
    fireball_max = _fireball_max;
    fireball_current = 0;
}

void Gui_update(float _fireball_current) {
    fireball_current = _fireball_current;
}

void Gui_render(sfRenderWindow* window) {
    sfRectangleShape* rect = sfRectangleShape_create();
    sfRectangleShape_setPosition(rect, (sfVector2f){ 50, 50 });
    sfRectangleShape_setSize(rect, (sfVector2f){ 100, 100 });
    sfRectangleShape_setFillColor(rect, (sfColor){ 255, 255, 255, 255 });
    sfRenderWindow_drawRectangleShape(window, rect, NULL);

    if (fireball_current < 100.f) {
        const float height = 100 * (fireball_current / fireball_max);
        sfRectangleShape_setPosition(rect, (sfVector2f){ 50, 50 + 100 - height });
        sfRectangleShape_setSize(rect, (sfVector2f){ 100, height });
        sfRectangleShape_setFillColor(rect, (sfColor){ 255, 0, 0, 100 });
        sfRenderWindow_drawRectangleShape(window, rect, NULL);
    }
}

