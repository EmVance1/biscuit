#include <SFML/Graphics.h>
#include <math.h>


sfVector2f sfVec2f_add(sfVector2f a, sfVector2f b) { return (sfVector2f){ .x=a.x + b.x, .y=a.y + b.y }; }
sfVector2f sfVec2f_sub(sfVector2f a, sfVector2f b) { return (sfVector2f){ .x=a.x - b.x, .y=a.y - b.y }; }
sfVector2f sfVec2f_scale(sfVector2f a, float b) { return (sfVector2f){ .x=a.x * b, .y=a.y * b }; }
float sfVec2f_dot(sfVector2f a, sfVector2f b) { return a.x * b.x + a.y * b.y; }
float sfVec2f_len(sfVector2f v) { return sqrtf(sfVec2f_dot(v, v)); }
sfVector2f sfVec2f_norm(sfVector2f v) { return sfVec2f_scale(v, 1.f / sfVec2f_len(v)); }
sfVector2f sfVec2f_compDiv(sfVector2f a, sfVector2f b) { return (sfVector2f){ .x=a.x / b.x, .y=a.y / b.y }; }
sfVector2f sfVec2f_compMul(sfVector2f a, sfVector2f b) { return (sfVector2f){ .x=a.x * b.x, .y=a.y * b.y }; }

sfVector2i sfVec2i_add(sfVector2i a, sfVector2i b) { return (sfVector2i){ .x=a.x + b.x, .y=a.y + b.y }; }
sfVector2i sfVec2i_sub(sfVector2i a, sfVector2i b) { return (sfVector2i){ .x=a.x - b.x, .y=a.y - b.y }; }
sfVector2f sfVec2i_scale(sfVector2i a, float b) { return (sfVector2f){ .x=(float)a.x * b, .y=(float)a.y * b }; }
int   sfVec2i_dot(sfVector2i a, sfVector2i b) { return a.x * b.x + a.y * b.y; }
float sfVec2i_len(sfVector2i v) { return sfVec2f_len((sfVector2f){ .x=(float)v.x, .y=(float)v.y }); }
sfVector2f sfVec2i_norm(sfVector2i v) { return sfVec2i_scale(v, 1.f / sfVec2i_len(v)); }
sfVector2f sfVec2i_compDiv(sfVector2i a, sfVector2i b) { return (sfVector2f){ .x=(float)a.x / (float)b.x, .y=(float)a.y / (float)b.y }; }
sfVector2i sfVec2i_compDivFloor(sfVector2i a, sfVector2i b) { return (sfVector2i){ .x=a.x / b.x, .y=a.y / b.y }; }
sfVector2i sfVec2i_compMul(sfVector2i a, sfVector2i b) { return (sfVector2i){ .x=a.x * b.x, .y=a.y * b.y }; }

sfVector2u sfVec2u_add(sfVector2u a, sfVector2u b) { return (sfVector2u){ .x=a.x + b.x, .y=a.y + b.y }; }
sfVector2u sfVec2u_sub(sfVector2u a, sfVector2u b) { return (sfVector2u){ .x=a.x - b.x, .y=a.y - b.y }; }
sfVector2f sfVec2u_scale(sfVector2u a, float b) { return (sfVector2f){ .x=(float)a.x * b, .y=(float)a.y * b }; }
int   sfVec2u_dot(sfVector2u a, sfVector2u b) { return a.x * b.x + a.y * b.y; }
float sfVec2u_len(sfVector2u v) { return sfVec2f_len((sfVector2f){ .x=(float)v.x, .y=(float)v.y }); }
sfVector2f sfVec2u_norm(sfVector2u v) { return sfVec2u_scale(v, 1.f / sfVec2u_len(v)); }
sfVector2f sfVec2u_compDiv(sfVector2u a, sfVector2u b) { return (sfVector2f){ .x=(float)a.x / (float)b.x, .y=(float)a.y / (float)b.y }; }
sfVector2u sfVec2u_compDivFloor(sfVector2u a, sfVector2u b) { return (sfVector2u){ .x=a.x / b.x, .y=a.y / b.y }; }
sfVector2u sfVec2u_compMul(sfVector2u a, sfVector2u b) { return (sfVector2u){ .x=a.x * b.x, .y=a.y * b.y }; }

