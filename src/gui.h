#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.h>


void Gui_init(float fireball_max);
void Gui_update(float fireball_current);
void Gui_render(sfRenderWindow* window);


#endif
