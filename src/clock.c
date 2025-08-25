#include "clock.h"
#include <SFML/Graphics.h>


static sfClock* s_clock;
static sfTime s_lastFrame;

void Clock_init() {
    s_clock = sfClock_create();
    Clock_setFrame();
}

void Clock_setFrame() {
    s_lastFrame = sfClock_restart(s_clock);
}

float Clock_deltaTime() {
    return sfTime_asSeconds(s_lastFrame);
}

