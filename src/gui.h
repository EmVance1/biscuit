#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.h>


void Gui_init(float hit_max, float dash_max, float fireball_max);
void Gui_update(float hit_current, float dash_current, float fireball_current);
void Gui_render(sfRenderWindow* window);


#endif
