#include "../origami.h"
#include <string.h>

int ou_button_ex(ou_context *ctx, const char *label, int icon, int opt) {
    int res = 0;
    ou_id id = label ? ou_hash(ctx, label, (int) strlen(label))
                     : ou_hash(ctx, &icon, sizeof(icon));
    ou_rect r = ou_allocate(ctx);
    ou_sense(ctx, id, r, opt);
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id) {
        res |= OU_SIGNAL_COMMIT;
        ou_notify(ctx, id, OU_SIGNAL_COMMIT);
    }
    ou_decorate(ctx, id, r, OU_TINT_CONTROL, opt);
    if (label) { ou_inscribe(ctx, label, r, OU_TINT_LABEL, opt); }
    if (icon)  {
        ou_paint_symbol(ctx, icon, r, ctx->theme->colors[OU_TINT_LABEL]);
    }
    return res;
}
