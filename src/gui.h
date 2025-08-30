#ifndef GUI_H
#define GUI_H
#include <SFML/Graphics.h>


void Gui_init(float attack_max, float dash_max, float fireball_max, float hazard_max);
void Gui_update(float attack_current, float dash_current, float fireball_current, float hazard_current);
void Gui_render(sfRenderWindow* window);


#endif
