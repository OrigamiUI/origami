#include "../origami.h"

int ou_switch_ex(ou_context *ctx, const char *label, int *state, int opt) {
    int res = 0;
    int track_w, track_h, knob_pad, knob_sz, knob_x;
    ou_rect r, track, knob;
    ou_id id = ou_hash(ctx, &state, sizeof(state));
    r = ou_allocate(ctx);
    ou_sense(ctx, id, r, opt);
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        res |= OU_SIGNAL_ALTERED;
        *state = !*state;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }
    /* draw track */
    track_h = r.h;
    track_w = track_h * 2;
    track = ou_rect_make(r.x, r.y, track_w, track_h);
    ou_paint_rect(ctx, track, *state
        ? ctx->theme->colors[OU_TINT_ACCENT]
        : ctx->theme->colors[OU_TINT_TRACK]);
    /* draw knob */
    knob_pad = 3;
    knob_sz = track_h - knob_pad * 2;
    knob_x = *state ? track.x + track_w - knob_sz - knob_pad
                    : track.x + knob_pad;
    knob = ou_rect_make(knob_x, track.y + knob_pad, knob_sz, knob_sz);
    ou_paint_rect(ctx, knob, ctx->theme->colors[OU_TINT_LABEL]);
    /* label */
    if (label) {
        ou_rect lr = ou_rect_make(r.x + track_w + ctx->theme->padding,
                                  r.y, r.w - track_w - ctx->theme->padding,
                                  r.h);
        ou_inscribe(ctx, label, lr, OU_TINT_LABEL, 0);
    }
    return res;
}
