#include "../origami.h"

int ou_select(ou_context *ctx, const char **items, int count, int *selected) {
    int res = 0;
    const char *label = (*selected >= 0 && *selected < count)
        ? items[*selected] : "---";
    ou_id id = ou_hash(ctx, &selected, sizeof(selected));
    ou_rect r = ou_allocate(ctx);
    ou_sense(ctx, id, r, 0);
    ou_decorate(ctx, id, r, OU_TINT_FIELD, 0);
    ou_inscribe(ctx, label, r, OU_TINT_LABEL, 0);
    /* draw dropdown arrow */
    ou_paint_symbol(ctx, OU_GLYPH_UNFOLDED,
        ou_rect_make(r.x + r.w - r.h, r.y, r.h, r.h),
        ctx->theme->colors[OU_TINT_LABEL]);
    /* open dropdown on click */
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        ou_frame *frm = ou_find_frame(ctx, "!select");
        ctx->hover_root = ctx->next_hover_root = frm;
        frm->rect = ou_rect_make(r.x, r.y + r.h + 2, r.w, 1);
        frm->open = 1;
        ou_elevate(ctx, frm);
    }
    /* dropdown items */
    {
        int opt = OU_HINT_OVERLAY | OU_HINT_SHRINK | OU_HINT_RIGID |
                  OU_HINT_FIXED | OU_HINT_HEADLESS | OU_HINT_HIDDEN;
        if (ou_window_begin_ex(ctx, "!select", ou_rect_make(0, 0, 0, 0),
                               opt))
        {
            int i;
            ou_row(ctx, 1,
                   (int[]) { r.w - ctx->theme->padding * 2 }, 0);
            for (i = 0; i < count; i++) {
                if (ou_button_ex(ctx, items[i], 0, 0)) {
                    if (*selected != i) {
                        *selected = i;
                        res |= OU_SIGNAL_ALTERED;
                    }
                    /* close the dropdown */
                    ou_active_frame(ctx)->open = 0;
                }
            }
            ou_window_end(ctx);
        }
    }
    return res;
}
