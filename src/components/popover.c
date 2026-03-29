#include "../origami.h"

void ou_summon_popover(ou_context *ctx, const char *name) {
    ou_frame *frm = ou_find_frame(ctx, name);
    ou_rect anchor = ctx->last_rect;
    ctx->hover_root = ctx->next_hover_root = frm;
    /* position below the anchor widget */
    frm->rect = ou_rect_make(anchor.x, anchor.y + anchor.h + 2, 1, 1);
    frm->open = 1;
    ou_elevate(ctx, frm);
}

int ou_popover_begin(ou_context *ctx, const char *name) {
    int opt = OU_HINT_OVERLAY | OU_HINT_SHRINK | OU_HINT_RIGID |
              OU_HINT_FIXED | OU_HINT_HEADLESS | OU_HINT_HIDDEN;
    return ou_window_begin_ex(ctx, name, ou_rect_make(0, 0, 0, 0), opt);
}

void ou_popover_end(ou_context *ctx) {
    ou_window_end(ctx);
}
