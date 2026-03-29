#include "../origami.h"

int ou_radio(ou_context *ctx, const char *label, int *selected, int value) {
    int res = 0;
    ou_rect r, box;
    ou_id id;
    ou_push_scope(ctx, &selected, sizeof(selected));
    id = ou_hash(ctx, &value, sizeof(value));
    r = ou_allocate(ctx);
    box = ou_rect_make(r.x, r.y, r.h, r.h);
    ou_sense(ctx, id, r, 0);
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        *selected = value;
        res |= OU_SIGNAL_ALTERED;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }
    ou_decorate(ctx, id, box, OU_TINT_FIELD, 0);
    if (*selected == value) {
        int inset = ou_max(box.w / 4, 2);
        ou_paint_rect(ctx,
            ou_rect_make(box.x + inset, box.y + inset,
                         box.w - inset * 2, box.h - inset * 2),
            ctx->theme->colors[OU_TINT_ACCENT]);
    }
    r = ou_rect_make(r.x + box.w, r.y, r.w - box.w, r.h);
    ou_inscribe(ctx, label, r, OU_TINT_LABEL, 0);
    ou_pop_scope(ctx);
    return res;
}
