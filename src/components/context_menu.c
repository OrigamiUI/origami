#include "../origami_internal.h"

void ou_open_context_menu(ou_context *ctx, const char *name) {
    ou_summon_popup(ctx, name);
}

int ou_context_menu_begin(ou_context *ctx, const char *name) {
    /* auto-open on right-click over the last widget */
    ou_rect anchor = ctx->last_rect;
    if (ctx->mouse_pressed & OU_MOUSE_SECONDARY &&
        ou__point_in_rect(anchor, ctx->mouse_pos) &&
        ou__in_hover_root(ctx))
    {
        ou_open_context_menu(ctx, name);
    }
    return ou_popup_begin(ctx, name);
}

void ou_context_menu_end(ou_context *ctx) {
    ou_popup_end(ctx);
}
