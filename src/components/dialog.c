#include "../origami_internal.h"

int ou_dialog_begin(ou_context *ctx, const char *title, int *open) {
    ou_frame *frm;
    ou_rect rect;
    int opt;
    if (!*open) { return 0; }
    /* draw fullscreen overlay backdrop */
    ou_paint_rect(ctx, ou__infinite_rect,
                  ctx->theme->colors[OU_TINT_OVERLAY]);
    /* centered dialog window */
    rect = ou_rect_make(0, 0, 340, 200);
    opt = OU_HINT_PERMANENT | OU_HINT_RIGID;
    if (!ou_window_begin_ex(ctx, title, rect, opt)) { return 0; }
    frm = ou_active_frame(ctx);
    /* center on screen if not yet positioned by user drag */
    if (frm->rect.x == 0 && frm->rect.y == 0) {
        frm->rect.x = 200;
        frm->rect.y = 150;
    }
    return OU_SIGNAL_ACTIVE;
}

void ou_dialog_end(ou_context *ctx) {
    ou_window_end(ctx);
}

int ou_alert_dialog(ou_context *ctx, const char *title, const char *message,
                    int *open)
{
    int res = 0;
    if (!ou_dialog_begin(ctx, title, open)) { return 0; }
    ou_row(ctx, 1, (int[]) { -1 }, 0);
    ou_paragraph(ctx, message);
    ou_separator(ctx);
    ou_row(ctx, 2, (int[]) { 0, 0 }, 0);
    if (ou_button_ex(ctx, "Cancel", 0, OU_HINT_CENTER)) {
        *open = 0;
    }
    if (ou_button_ex(ctx, "Confirm", 0, OU_HINT_CENTER)) {
        *open = 0;
        res |= OU_SIGNAL_COMMIT;
    }
    ou_dialog_end(ctx);
    return res;
}

int ou_confirm(ou_context *ctx, const char *title, const char *message,
               int *open)
{
    return ou_alert_dialog(ctx, title, message, open);
}
