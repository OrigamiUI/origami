#include "../origami.h"

void ou_scroll_area_begin_ex(ou_context *ctx, const char *name, int height,
                             int opt)
{
    int w = -1;
    ou_row(ctx, 1, &w, height);
    ou_panel_begin_ex(ctx, name, opt);
}

void ou_scroll_area_end(ou_context *ctx) {
    ou_panel_end(ctx);
}
