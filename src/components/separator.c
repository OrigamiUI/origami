#include "../origami.h"

void ou_separator(ou_context *ctx) {
    ou_rect r;
    int width = -1;
    int h = ctx->theme->spacing * 2 + 1;
    ou_row(ctx, 1, &width, h);
    r = ou_allocate(ctx);
    ou_paint_rect(ctx, ou_rect_make(r.x, r.y + r.h / 2, r.w, 1),
                  ctx->theme->colors[OU_TINT_CONTROL]);
}
