#include <SFML/Graphics.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "Game.h"
#include "atlas.h"
#include "clock.h"
#include "tilemap.h"
#include "world.h"

#include <navmesh/c/lib.h>
#include "pathtracker.h"

#ifdef VANGO_DEBUG
    #define SCREEN_MODE sfDefaultStyle
#else
    #define SCREEN_MODE sfFullscreen
#endif


World sandboxMap(const sfuTextureAtlas* atlas) {
    const static uint32_t map[] = {
        3, 0,  1, 2,  1, 2,  1, 2,  1, 2,  1, 2,  1, 2,  1, 2,  1, 2,  4, 0,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        2, 1,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 3,  0, 1,
        3, 1,  1, 0,  1, 0,  1, 0,  1, 0,  1, 0,  1, 0,  1, 0,  1, 0,  4, 1,
    };

    return World_createFromIndices(map, (sfVector2u){ 10, 10 }, atlas);
}


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

    sfuTextureAtlas* tileatlas = sfuTextureAtlas_createFromFile("res/textures/tilesheet.png", (sfVector2u){ 8, 8 });
    World sandbox = sandboxMap(tileatlas);

    PathTracker* tracker = PathTracker_create(sandbox.navmesh);

    Game_Init(&sandbox);

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

        Game_Update(camera);

        sfRenderWindow_clear(window, (sfColor){ 0, 0, 0, 255 });
        sfRenderWindow_setView(window, camera);

        sfRenderWindow_drawTileMap(window, sandbox.background, NULL);

        Game_Render(window);

        sfRenderWindow_display(window);
    }

    PathTracker_free(tracker);

    // Game_Destroy();

    World_free(&sandbox);
    sfuTextureAtlas_free(tileatlas);
    sfView_destroy(camera);
    sfRenderWindow_destroy(window);
}

