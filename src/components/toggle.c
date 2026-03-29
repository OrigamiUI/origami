#include "../origami.h"

int ou_toggle_ex(ou_context *ctx, const char *label, int *state, int opt) {
    int res = 0;
    ou_id id = ou_hash(ctx, &state, sizeof(state));
    ou_rect r = ou_allocate(ctx);
    ou_sense(ctx, id, r, opt);
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        res |= OU_SIGNAL_ALTERED;
        *state = !*state;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }
    if (*state) {
        ctx->draw_frame(ctx, r, OU_TINT_ACCENT);
    } else {
        ou_decorate(ctx, id, r, OU_TINT_CONTROL, opt);
    }
    if (label) { ou_inscribe(ctx, label, r, OU_TINT_LABEL, opt); }
    return res;
}
