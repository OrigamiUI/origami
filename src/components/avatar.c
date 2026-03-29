#include "../origami_internal.h"

void ou_avatar_ex(ou_context *ctx, const char *initials, ou_color bg,
                  int opt)
{
    ou_rect r = ou_allocate(ctx);
    int sz = ou_min(r.w, r.h);
    ou_rect box = ou_rect_make(r.x + (r.w - sz) / 2,
                               r.y + (r.h - sz) / 2, sz, sz);
    unused(opt);
    ou_paint_rect(ctx, box, bg);
    ou_inscribe(ctx, initials, box, OU_TINT_LABEL, OU_HINT_CENTER);
}
