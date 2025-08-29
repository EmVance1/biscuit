#include "clock.h"
#include <SFML/Graphics.h>


static sfClock* s_clock;
static sfTime s_lastFrame;
static sfTime s_thisFrame;

void Clock_init() {
    s_clock = sfClock_create();
    s_lastFrame = sfSeconds(0);
    s_thisFrame = sfSeconds(0);
}

void Clock_setFrame() {
    s_lastFrame = s_thisFrame;
    s_thisFrame = sfClock_getElapsedTime(s_clock);
}

float Clock_deltaTime() {
    return sfTime_asSeconds(s_thisFrame) - sfTime_asSeconds(s_lastFrame);
}

float Clock_totalTime() {
    return sfTime_asSeconds(s_thisFrame);
}

