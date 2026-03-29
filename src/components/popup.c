#include "../origami.h"

void ou_summon_popup(ou_context *ctx, const char *name) {
    ou_frame *frm = ou_find_frame(ctx, name);
    ctx->hover_root = ctx->next_hover_root = frm;
    frm->rect = ou_rect_make(ctx->mouse_pos.x, ctx->mouse_pos.y, 1, 1);
    frm->open = 1;
    ou_elevate(ctx, frm);
}

int ou_popup_begin(ou_context *ctx, const char *name) {
    int opt = OU_HINT_OVERLAY | OU_HINT_SHRINK | OU_HINT_RIGID |
              OU_HINT_FIXED | OU_HINT_HEADLESS | OU_HINT_HIDDEN;
    return ou_window_begin_ex(ctx, name, ou_rect_make(0, 0, 0, 0), opt);
}

void ou_popup_end(ou_context *ctx) {
    ou_window_end(ctx);
}
