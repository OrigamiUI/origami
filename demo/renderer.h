#ifndef RENDERER_H
#define RENDERER_H

#include "origami.h"

void r_init(void);
void r_draw_rect(ou_rect rect, ou_color color);
void r_draw_text(const char *text, ou_vec2 pos, ou_color color);
void r_draw_icon(int id, ou_rect rect, ou_color color);
 int r_get_text_width(const char *text, int len);
 int r_get_text_height(void);
void r_set_clip_rect(ou_rect rect);
void r_clear(ou_color color);
void r_present(void);

#endif
