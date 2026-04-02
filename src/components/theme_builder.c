#include "../origami_internal.h"

void ou_theme_builder_init(ou_theme_builder *b) {
    b->theme = ou__base_theme;
}

void ou_theme_builder_font(ou_theme_builder *b, ou_font font) {
    b->theme.font = font;
}

void ou_theme_builder_padding(ou_theme_builder *b, int padding) {
    b->theme.padding = padding;
}

void ou_theme_builder_spacing(ou_theme_builder *b, int spacing) {
    b->theme.spacing = spacing;
}

void ou_theme_builder_color(ou_theme_builder *b, int tint, ou_color c) {
    if (tint >= 0 && tint < OU_TINT_COUNT) {
        b->theme.colors[tint] = c;
    }
}

void ou_theme_builder_preset_dark(ou_theme_builder *b) {
    b->theme.colors[OU_TINT_LABEL]        = ou_color_make(230, 230, 230, 255);
    b->theme.colors[OU_TINT_EDGE]         = ou_color_make( 25,  25,  25, 255);
    b->theme.colors[OU_TINT_CANVAS]       = ou_color_make( 50,  50,  50, 255);
    b->theme.colors[OU_TINT_HEADER]       = ou_color_make( 25,  25,  25, 255);
    b->theme.colors[OU_TINT_HEADING]      = ou_color_make(240, 240, 240, 255);
    b->theme.colors[OU_TINT_SURFACE]      = ou_color_make(  0,   0,   0,   0);
    b->theme.colors[OU_TINT_CONTROL]      = ou_color_make( 75,  75,  75, 255);
    b->theme.colors[OU_TINT_CONTROL_WARM] = ou_color_make( 95,  95,  95, 255);
    b->theme.colors[OU_TINT_CONTROL_HOT]  = ou_color_make(115, 115, 115, 255);
    b->theme.colors[OU_TINT_FIELD]        = ou_color_make( 30,  30,  30, 255);
    b->theme.colors[OU_TINT_FIELD_WARM]   = ou_color_make( 35,  35,  35, 255);
    b->theme.colors[OU_TINT_FIELD_HOT]    = ou_color_make( 40,  40,  40, 255);
    b->theme.colors[OU_TINT_TRACK]        = ou_color_make( 43,  43,  43, 255);
    b->theme.colors[OU_TINT_HANDLE]       = ou_color_make( 30,  30,  30, 255);
    b->theme.colors[OU_TINT_ACCENT]       = ou_color_make( 75, 145, 215, 255);
    b->theme.colors[OU_TINT_OVERLAY]      = ou_color_make(  0,   0,   0, 128);
}

void ou_theme_builder_preset_light(ou_theme_builder *b) {
    b->theme.colors[OU_TINT_LABEL]        = ou_color_make( 30,  30,  30, 255);
    b->theme.colors[OU_TINT_EDGE]         = ou_color_make(190, 190, 190, 255);
    b->theme.colors[OU_TINT_CANVAS]       = ou_color_make(245, 245, 245, 255);
    b->theme.colors[OU_TINT_HEADER]       = ou_color_make(225, 225, 225, 255);
    b->theme.colors[OU_TINT_HEADING]      = ou_color_make( 20,  20,  20, 255);
    b->theme.colors[OU_TINT_SURFACE]      = ou_color_make(255, 255, 255,   0);
    b->theme.colors[OU_TINT_CONTROL]      = ou_color_make(200, 200, 200, 255);
    b->theme.colors[OU_TINT_CONTROL_WARM] = ou_color_make(180, 180, 180, 255);
    b->theme.colors[OU_TINT_CONTROL_HOT]  = ou_color_make(160, 160, 160, 255);
    b->theme.colors[OU_TINT_FIELD]        = ou_color_make(255, 255, 255, 255);
    b->theme.colors[OU_TINT_FIELD_WARM]   = ou_color_make(240, 240, 240, 255);
    b->theme.colors[OU_TINT_FIELD_HOT]    = ou_color_make(230, 230, 230, 255);
    b->theme.colors[OU_TINT_TRACK]        = ou_color_make(210, 210, 210, 255);
    b->theme.colors[OU_TINT_HANDLE]       = ou_color_make(170, 170, 170, 255);
    b->theme.colors[OU_TINT_ACCENT]       = ou_color_make( 50, 120, 200, 255);
    b->theme.colors[OU_TINT_OVERLAY]      = ou_color_make(  0,   0,   0,  80);
}

ou_theme ou_theme_builder_build(ou_theme_builder *b) {
    return b->theme;
}
