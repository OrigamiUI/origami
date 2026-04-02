#include "../origami_internal.h"

void ou_toolbar_begin(ou_context *ctx) {
    int w = -1;
    ou_row(ctx, 1, &w, 0);
    ou_column_begin(ctx);
    ou_paint_rect(ctx, ou__current_grid(ctx)->body,
                  ctx->theme->colors[OU_TINT_HEADER]);
}

void ou_toolbar_end(ou_context *ctx) {
    ou_column_end(ctx);
}
