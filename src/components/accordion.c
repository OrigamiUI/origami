#include "../origami.h"
#include <string.h>

int ou_accordion_item(ou_context *ctx, const char *label, int *active,
                      int index)
{
    ou_rect r;
    int expanded;
    int w = -1;
    ou_id id = ou_hash(ctx, label, (int) strlen(label));
    ou_row(ctx, 1, &w, 0);
    r = ou_allocate(ctx);
    ou_sense(ctx, id, r, 0);
    expanded = (*active == index);
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        *active = expanded ? -1 : index;
        expanded = !expanded;
    }
    ou_decorate(ctx, id, r, OU_TINT_CONTROL, 0);
    ou_paint_symbol(ctx, expanded ? OU_GLYPH_UNFOLDED : OU_GLYPH_FOLDED,
        ou_rect_make(r.x, r.y, r.h, r.h),
        ctx->theme->colors[OU_TINT_LABEL]);
    r.x += r.h - ctx->theme->padding;
    r.w -= r.h - ctx->theme->padding;
    ou_inscribe(ctx, label, r, OU_TINT_LABEL, 0);
    return expanded ? OU_SIGNAL_ACTIVE : 0;
}
