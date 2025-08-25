#include "SFML/System/Time.h"
#include <SFML/Graphics.h>
#include <stdio.h>


#ifdef VANGO_DEBUG
    #define VIDEO_MODE (sfVideoMode){ .width=1920, .height=1080, .bitsPerPixel=32 }
    #define SCREEN_MODE sfDefaultStyle
#else
    #define VIDEO_MODE sfVideoMode_isValid((sfVideoMode){ .width=1920, .height=1080, .bitsPerPixel=32 }) ? \
        (sfVideoMode){ .width=1920, .height=1080, .bitsPerPixel=32 } : sfVideoMode_getDesktopMode()
    #define SCREEN_MODE sfFullscreen
#endif


int main() {
    sfRenderWindow* window = sfRenderWindow_create(VIDEO_MODE, "Biscuit", SCREEN_MODE, NULL);
    sfClock* clock = sfClock_create();

    while (sfRenderWindow_isOpen(window)) {
        const float deltatime = sfTime_asSeconds(sfClock_restart(clock));

        sfEvent event;
        while (sfWindow_pollEvent((sfWindow*)window, &event)) {
            switch (event.type) {
            case sfEvtClosed:
                sfRenderWindow_close(window);
                break;
            default:
                break;
            }
        }

        sfRenderWindow_clear(window, (sfColor){ 255, 0, 255, 255 });
        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);
}

