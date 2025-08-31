#include <SFML/Graphics.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "utils/clock.h"
#include "utils/atlas.h"
#include "utils/tilemap.h"
#include "Game.h"
#include "world.h"
#include "levels.h"
#include "gui.h"

#include <navmesh/c/lib.h>
#include "pathtracker.h"

#ifdef VANGO_DEBUG
    #define SCREEN_MODE sfDefaultStyle
#else
    #define SCREEN_MODE sfFullscreen
#endif



int main() {
    sfVideoMode videomode = sfVideoMode_getDesktopMode();
    sfRenderWindow* window = sfRenderWindow_create(videomode, "Biscuit", SCREEN_MODE, NULL);
    srand((uint32_t)time(NULL));
    Clock_init();

    sfView* camera = sfView_createFromRect((sfFloatRect){ 0, 0, 480, 270 });
    const float aspectratio = (float)videomode.width / (float)videomode.height;
    if (aspectratio < 16.f / 9.f) {
        const float ratio = ((float)videomode.width / (16.f / 9.f)) / (float)videomode.height;
        sfView_setViewport(camera, (sfFloatRect){ 0.f, 0.5f * (1.f - ratio), 1.f, ratio });
    } else if (aspectratio > 16.f / 9.f) {
        const float ratio = ((float)videomode.height / (9.f / 16.f)) / (float)videomode.width;
        sfView_setViewport(camera, (sfFloatRect){ 0.5f * (1.f - ratio), 0.f, ratio, 1.f });
    }

    sfView* guiview = sfView_createFromRect((sfFloatRect){ 0, 0, 1920, 1080 });
    const float guiaspectratio = (float)videomode.width / (float)videomode.height;
    if (guiaspectratio < 16.f / 9.f) {
        const float ratio = ((float)videomode.width / (16.f / 9.f)) / (float)videomode.height;
        sfView_setViewport(guiview, (sfFloatRect){ 0.f, 0.5f * (1.f - ratio), 1.f, ratio });
    } else if (guiaspectratio > 16.f / 9.f) {
        const float ratio = ((float)videomode.height / (9.f / 16.f)) / (float)videomode.width;
        sfView_setViewport(guiview, (sfFloatRect){ 0.5f * (1.f - ratio), 0.f, ratio, 1.f });
    }

    Game_Init();

    while (sfRenderWindow_isOpen(window)) {
        Clock_setFrame();

        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            switch (event.type) {
            case sfEvtClosed:
                sfRenderWindow_close(window);
                break;
#ifdef VANGO_DEBUG
            case sfEvtKeyPressed:
                if (sfKeyboard_isKeyPressed(sfKeyEscape))
                    sfRenderWindow_close(window);
                break;
#endif
            default:
                break;
            }
        }

        Game_Update(window, camera);

        sfRenderWindow_clear(window, (sfColor){ 75, 62, 42, 255 });
        sfRenderWindow_setView(window, camera);
        Game_Render(window);
        sfRenderWindow_setView(window, guiview);
        Gui_render(window);

        sfRenderWindow_display(window);
    }

    // Game_Destroy();

    // World_free(&sandbox);
    // sfuTextureAtlas_free(tileatlas);
    sfView_destroy(camera);
    sfRenderWindow_destroy(window);
}

