#include "../origami_internal.h"

void ou_panel_begin_ex(ou_context *ctx, const char *name, int opt) {
    ou_frame *frm;
    ou_push_scope(ctx, name, (int) strlen(name));
    frm = ou__resolve_frame(ctx, ctx->last_id, opt);
    frm->rect = ou_allocate(ctx);
    if (~opt & OU_HINT_BARE) {
        ctx->draw_frame(ctx, frm->rect, OU_TINT_SURFACE);
    }
    push(ctx->frame_stack, frm);
    ou__push_frame_body(ctx, frm, frm->rect, opt);
    ou_push_scissor(ctx, frm->body);
    ou_clip(ctx, ou_current_scissor(ctx));
}

void ou_panel_end(ou_context *ctx) {
    ou_pop_scissor(ctx);
    ou_clip(ctx, ou_current_scissor(ctx));
    ou__pop_frame_state(ctx);
}
