#include "../origami_internal.h"

void ou_checkbox_group_begin(ou_context *ctx, const char *label) {
    int w = -1;
    ou_push_scope(ctx, label, (int) strlen(label));
    ou_row(ctx, 1, &w, 0);
    ou_inscribe(ctx, label, ou_allocate(ctx), OU_TINT_LABEL, 0);
    ou__current_grid(ctx)->indent += ctx->theme->indent;
}

void ou_checkbox_group_end(ou_context *ctx) {
    ou__current_grid(ctx)->indent -= ctx->theme->indent;
    ou_pop_scope(ctx);
}
