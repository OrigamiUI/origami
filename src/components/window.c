#include "../origami_internal.h"

int ou_window_begin_ex(ou_context *ctx, const char *title, ou_rect rect,
                       int opt)
{
    ou_rect body;
    ou_id id = ou_hash(ctx, title, (int) strlen(title));
    ou_frame *frm = ou__resolve_frame(ctx, id, opt);
    if (!frm || !frm->open) { return 0; }
    push(ctx->scope_stack, id);

    if (frm->rect.w == 0) { frm->rect = rect; }
    ou__begin_root(ctx, frm);
    rect = body = frm->rect;

    /* draw window background */
    if (~opt & OU_HINT_BARE) {
        ctx->draw_frame(ctx, rect, OU_TINT_CANVAS);
    }

    /* title bar */
    if (~opt & OU_HINT_HEADLESS) {
        ou_rect tr = rect;
        tr.h = ctx->theme->title_height;
        ctx->draw_frame(ctx, tr, OU_TINT_HEADER);

        /* title text + dragging */
        if (~opt & OU_HINT_HEADLESS) {
            ou_id tid = ou_hash(ctx, "!title", 6);
            ou_sense(ctx, tid, tr, opt);
            ou_inscribe(ctx, title, tr, OU_TINT_HEADING, opt);
            if (tid == ctx->focus && ctx->mouse_down == OU_MOUSE_PRIMARY) {
                frm->rect.x += ctx->mouse_delta.x;
                frm->rect.y += ctx->mouse_delta.y;
            }
            body.y += tr.h;
            body.h -= tr.h;
        }

        /* close button */
        if (~opt & OU_HINT_PERMANENT) {
            ou_id cid = ou_hash(ctx, "!close", 6);
            ou_rect cr = ou_rect_make(tr.x + tr.w - tr.h, tr.y,
                                      tr.h, tr.h);
            tr.w -= cr.w;
            ou_paint_symbol(ctx, OU_GLYPH_DISMISS, cr,
                            ctx->theme->colors[OU_TINT_HEADING]);
            ou_sense(ctx, cid, cr, opt);
            if (ctx->mouse_pressed == OU_MOUSE_PRIMARY &&
                cid == ctx->focus)
            {
                frm->open = 0;
            }
        }
    }

    ou__push_frame_body(ctx, frm, body, opt);

    /* resize handle */
    if (~opt & OU_HINT_RIGID) {
        int sz = ctx->theme->title_height;
        ou_id rid = ou_hash(ctx, "!resize", 7);
        ou_rect rr = ou_rect_make(rect.x + rect.w - sz,
                                  rect.y + rect.h - sz, sz, sz);
        ou_sense(ctx, rid, rr, opt);
        if (rid == ctx->focus && ctx->mouse_down == OU_MOUSE_PRIMARY) {
            frm->rect.w = ou_max(96, frm->rect.w + ctx->mouse_delta.x);
            frm->rect.h = ou_max(64, frm->rect.h + ctx->mouse_delta.y);
        }
    }

    /* auto-size to content */
    if (opt & OU_HINT_SHRINK) {
        ou_rect lr = ou__current_grid(ctx)->body;
        frm->rect.w = frm->content_size.x + (frm->rect.w - lr.w);
        frm->rect.h = frm->content_size.y + (frm->rect.h - lr.h);
    }

    /* close popup on outside click */
    if (opt & OU_HINT_OVERLAY && ctx->mouse_pressed &&
        ctx->hover_root != frm)
    {
        frm->open = 0;
    }

    ou_push_scissor(ctx, frm->body);
    ou_clip(ctx, frm->body);
    return OU_SIGNAL_ACTIVE;
}

void ou_window_end(ou_context *ctx) {
    ou_pop_scissor(ctx);
    ou_clip(ctx, ou__infinite_rect);
    ou__end_root(ctx);
}
