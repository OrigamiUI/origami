#include "../origami.h"
#include <string.h>

int ou_tabs(ou_context *ctx, const char **labels, int count, int *selected) {
    int i, res = 0;
    int widths[OU_MAX_COLUMNS];
    if (count > OU_MAX_COLUMNS) { count = OU_MAX_COLUMNS; }
    for (i = 0; i < count; i++) { widths[i] = 0; }
    ou_row(ctx, count, widths, 0);
    for (i = 0; i < count; i++) {
        ou_id id = ou_hash(ctx, labels[i], (int) strlen(labels[i]));
        ou_rect r = ou_allocate(ctx);
        ou_sense(ctx, id, r, 0);
        if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id
            && *selected != i)
        {
            *selected = i;
            res |= OU_SIGNAL_ALTERED;
        }
        if (*selected == i) {
            ctx->draw_frame(ctx, r, OU_TINT_HEADER);
            ou_paint_rect(ctx,
                ou_rect_make(r.x, r.y + r.h - 2, r.w, 2),
                ctx->theme->colors[OU_TINT_ACCENT]);
        } else {
            ou_decorate(ctx, id, r, OU_TINT_CONTROL, 0);
        }
        ou_inscribe(ctx, labels[i], r, OU_TINT_LABEL, OU_HINT_CENTER);
    }
    return res;
}
