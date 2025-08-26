#include <SFML/Graphics.h>
#include "Game.h"
#include "clock.h"


#ifdef VANGO_DEBUG
    #define SCREEN_MODE sfDefaultStyle
#else
    #define SCREEN_MODE sfFullscreen
#endif


int main() {
    sfVideoMode videomode = sfVideoMode_getDesktopMode();
    sfRenderWindow* window = sfRenderWindow_create(videomode, "Biscuit", SCREEN_MODE, NULL);
    Clock_init();

    sfView* camera = sfView_createFromRect((sfFloatRect){ 0, 0, 1920, 1080 });
    const float aspectratio = (float)videomode.width / (float)videomode.height;
    if (aspectratio < 16.f / 9.f) {
        const float ratio = ((float)videomode.width / (16.f / 9.f)) / (float)videomode.height;
        sfView_setViewport(camera, (sfFloatRect){ 0.f, 0.5f * (1.f - ratio), 1.f, ratio });
    } else if (aspectratio > 16.f / 9.f) {
        const float ratio = ((float)videomode.height / (9.f / 16.f)) / (float)videomode.width;
        sfView_setViewport(camera, (sfFloatRect){ 0.5f * (1.f - ratio), 0.f, ratio, 1.f });
    }

    

    while (sfRenderWindow_isOpen(window)) {
        sfRenderWindow_clear(window, (sfColor){ 255, 255, 255, 255 });
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
                processKeyClicked(event);
                break;
            default:
                break;
            }
        }

        GameUpdate();

        sfRenderWindow_setView(window, camera);

        GameRender(window);

        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);
}

