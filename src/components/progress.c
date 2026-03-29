#include "../origami.h"
#include <stdio.h>

void ou_progress_ex(ou_context *ctx, ou_real value, int opt) {
    char buf[OU_FMT_LIMIT + 1];
    ou_rect r = ou_allocate(ctx);
    ou_real v = ou_clamp(value, 0, 1);
    int fill_w = (int)(v * r.w);
    ctx->draw_frame(ctx, r, OU_TINT_TRACK);
    if (fill_w > 0) {
        ou_paint_rect(ctx, ou_rect_make(r.x, r.y, fill_w, r.h),
                      ctx->theme->colors[OU_TINT_ACCENT]);
    }
    sprintf(buf, "%d%%", (int)(v * 100));
    ou_inscribe(ctx, buf, r, OU_TINT_LABEL, opt | OU_HINT_CENTER);
}

void ou_meter_ex(ou_context *ctx, ou_real value, ou_real low, ou_real high,
                 int opt)
{
    char buf[OU_FMT_LIMIT + 1];
    ou_rect r = ou_allocate(ctx);
    ou_real range = high - low;
    ou_real v = ou_clamp(value, low, high);
    ou_real ratio = (range > 0) ? (v - low) / range : 0;
    int fill_w = (int)(ratio * r.w);
    ctx->draw_frame(ctx, r, OU_TINT_TRACK);
    if (fill_w > 0) {
        ou_paint_rect(ctx, ou_rect_make(r.x, r.y, fill_w, r.h),
                      ctx->theme->colors[OU_TINT_ACCENT]);
    }
    sprintf(buf, OU_SLIDER_FMT, v);
    ou_inscribe(ctx, buf, r, OU_TINT_LABEL, opt | OU_HINT_CENTER);
}
