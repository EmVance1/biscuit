#include <SFML/Graphics.h>
#include "atlas.h"
#include "clock.h"
#include "tilemap.h"

#include <navmesh/c/lib.h>


#ifdef VANGO_DEBUG
    #define SCREEN_MODE sfDefaultStyle
#else
    #define SCREEN_MODE sfFullscreen
#endif


sfuTileMap* sandboxMap(const sfuTextureAtlas* atlas) {
    static uint32_t map[] = {
        3, 0,  1, 2,  1, 2,  1, 2,  1, 2,  1, 2,  1, 2,  4, 0,
        2, 1,  0, 3,  1, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  1, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  1, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  1, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  1, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  1, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        3, 1,  1, 0,  1, 0,  1, 0,  1, 0,  1, 0,  1, 0,  4, 1,
    };

    return sfuTileMap_createFromIndices(atlas, (uint32_t*)map, (sfVector2u){ 8, 8 });
}


int main() {
    sfVideoMode videomode = sfVideoMode_getDesktopMode();
    sfRenderWindow* window = sfRenderWindow_create(videomode, "Biscuit", SCREEN_MODE, NULL);
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

    sfuTextureAtlas* tileatlas = sfuTextureAtlas_createFromFile("res/textures/tilesheet.png", (sfVector2u){ 8, 8 });
    sfuTileMap* sandbox = sandboxMap(tileatlas);

    while (sfRenderWindow_isOpen(window)) {
        Clock_setFrame();

        sfEvent event;
        while (sfWindow_pollEvent((sfWindow*)window, &event)) {
            switch (event.type) {
            case sfEvtClosed:
                sfRenderWindow_close(window);
                break;
            case sfEvtKeyPressed:
                if (sfKeyboard_isKeyPressed(sfKeyEscape))
                    sfRenderWindow_close(window);
                break;
            default:
                break;
            }
        }

        sfRenderWindow_setView(window, camera);
        sfRenderWindow_clear(window, (sfColor){ 0, 0, 0, 255 });
        sfRenderWindow_drawTileMap(window, sandbox, NULL);
        sfRenderWindow_display(window);
    }

    sfuTileMap_free(sandbox);
    sfuTextureAtlas_free(tileatlas);
    sfView_destroy(camera);
    sfRenderWindow_destroy(window);
}

