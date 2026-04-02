#include "../origami_internal.h"

void ou_tooltip(ou_context *ctx, const char *text) {
    ou_rect anchor = ctx->last_rect;
    if (!ou__point_in_rect(anchor, ctx->mouse_pos)) { return; }
    if (!ou__in_hover_root(ctx)) { return; }
    /* render tooltip below the hovered widget */
    {
        ou_frame *saved;
        ou_font font = ctx->theme->font;
        int tw = ctx->text_width(font, text, -1);
        int th = ctx->text_height(font);
        int pad = ctx->theme->padding;
        int tx = anchor.x;
        int ty = anchor.y + anchor.h + 4;
        ou_rect bg = ou_rect_make(tx - pad, ty - pad,
                                  tw + pad * 2, th + pad * 2);

        /* render in a dedicated overlay root for correct z-ordering */
        ou_id id = ou_hash(ctx, "!tip", 4);
        ou_frame *frm = ou__resolve_frame(ctx, id, 0);
        frm->rect = ou__grow_rect(bg, 1);
        frm->open = 1;
        ou_elevate(ctx, frm);

        /* push scope, root, and grid for stack balance with ou__end_root */
        push(ctx->scope_stack, id);
        saved = ctx->next_hover_root;
        ou__begin_root(ctx, frm);
        ctx->next_hover_root = saved; /* tooltip must not steal hover */
        ou__push_grid(ctx, bg, ou_vec2_make(0, 0));

        ou_paint_rect(ctx, bg, ctx->theme->colors[OU_TINT_HEADER]);
        ou_paint_outline(ctx, ou__grow_rect(bg, 1),
                         ctx->theme->colors[OU_TINT_EDGE]);
        ou_paint_text(ctx, font, text, -1, ou_vec2_make(tx, ty),
                      ctx->theme->colors[OU_TINT_LABEL]);

        ou__end_root(ctx);
    }
}
