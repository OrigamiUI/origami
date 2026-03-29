#include "../origami.h"

int ou_checkbox(ou_context *ctx, const char *label, int *state) {
    int res = 0;
    ou_id id = ou_hash(ctx, &state, sizeof(state));
    ou_rect r = ou_allocate(ctx);
    ou_rect box = ou_rect_make(r.x, r.y, r.h, r.h);
    ou_sense(ctx, id, r, 0);
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        res |= OU_SIGNAL_ALTERED;
        *state = !*state;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }
    ou_decorate(ctx, id, box, OU_TINT_FIELD, 0);
    if (*state) {
        ou_paint_symbol(ctx, OU_GLYPH_TICK, box,
                        ctx->theme->colors[OU_TINT_LABEL]);
    }
    r = ou_rect_make(r.x + box.w, r.y, r.w - box.w, r.h);
    ou_inscribe(ctx, label, r, OU_TINT_LABEL, 0);
    return res;
}
