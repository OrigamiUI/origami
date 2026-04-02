#include "../origami_internal.h"

int ou_preview_card_begin(ou_context *ctx, const char *name) {
    ou_rect anchor = ctx->last_rect;
    int opt, show;
    ou_id id = ou_hash(ctx, name, (int) strlen(name));
    int idx = ou_cache_find(ctx, ctx->frame_pool, OU_FRAME_POOL_CAP, id);
    ou_frame *existing = (idx >= 0) ? &ctx->frames[idx] : NULL;
    int card_open = existing && existing->open;

    /* show when hovering anchor (in hover root, or card owns hover root)
       or when the mouse moves onto the card itself */
    show = 0;
    if (ou__point_in_rect(anchor, ctx->mouse_pos)) {
        if (ou__in_hover_root(ctx) ||
            (card_open && ctx->hover_root == existing))
        {
            show = 1;
        }
    } else if (card_open &&
               ou__point_in_rect(existing->rect, ctx->mouse_pos)) {
        show = 1;
    }

    if (!show) {
        int h_opt = OU_HINT_OVERLAY | OU_HINT_SHRINK | OU_HINT_RIGID |
                    OU_HINT_FIXED | OU_HINT_HEADLESS | OU_HINT_HIDDEN;
        ou_frame *frm = ou__resolve_frame(ctx, id, h_opt);
        if (frm) { frm->open = 0; }
        return 0;
    }
    /* position card below anchor */
    {
        ou_frame *frm = ou_find_frame(ctx, name);
        if (!card_open) {
            frm->rect = ou_rect_make(anchor.x,
                                     anchor.y + anchor.h + 4, 1, 1);
            frm->open = 1;
            ou_elevate(ctx, frm);
            ctx->hover_root = ctx->next_hover_root = frm;
        }
    }
    opt = OU_HINT_SHRINK | OU_HINT_RIGID | OU_HINT_FIXED |
          OU_HINT_HEADLESS | OU_HINT_HIDDEN;
    return ou_window_begin_ex(ctx, name, ou_rect_make(0, 0, 0, 0), opt);
}

void ou_preview_card_end(ou_context *ctx) {
    ou_window_end(ctx);
}
