#include "../origami_internal.h"

int ou_section_ex(ou_context *ctx, const char *label, int opt) {
    return ou__collapsible(ctx, label, 0, opt);
}

int ou_tree_begin_ex(ou_context *ctx, const char *label, int opt) {
    int res = ou__collapsible(ctx, label, 1, opt);
    if (res & OU_SIGNAL_ACTIVE) {
        ou__current_grid(ctx)->indent += ctx->theme->indent;
        push(ctx->scope_stack, ctx->last_id);
    }
    return res;
}

void ou_tree_end(ou_context *ctx) {
    ou__current_grid(ctx)->indent -= ctx->theme->indent;
    ou_pop_scope(ctx);
}

int ou_collapsible_begin_ex(ou_context *ctx, const char *label, int opt) {
    int res = ou__collapsible(ctx, label, 1, opt);
    if (res & OU_SIGNAL_ACTIVE) {
        push(ctx->scope_stack, ctx->last_id);
    }
    return res;
}

void ou_collapsible_end(ou_context *ctx) {
    ou_pop_scope(ctx);
}
