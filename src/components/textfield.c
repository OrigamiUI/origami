#include "../origami_internal.h"

enum { OU_TFIELD_IDLE, OU_TFIELD_FOCUSED };

typedef struct {
    int  (*handle)(ou_context *ctx, char *buf, int bufsz, ou_id id,
                   ou_rect r, int opt);
    void (*render)(ou_context *ctx, const char *buf, ou_id id, ou_rect r,
                   int opt);
} ou_tfield_state_vtable;


static int handle_idle(ou_context *ctx, char *buf, int bufsz, ou_id id,
                       ou_rect r, int opt)
{
    unused(ctx); unused(buf); unused(bufsz); unused(id); unused(r); unused(opt);
    return 0;
}

static void render_idle(ou_context *ctx, const char *buf, ou_id id,
                        ou_rect r, int opt)
{
    ou_decorate(ctx, id, r, OU_TINT_FIELD, opt);
    ou_inscribe(ctx, buf, r, OU_TINT_LABEL, opt);
}


static int handle_focused(ou_context *ctx, char *buf, int bufsz, ou_id id,
                          ou_rect r, int opt)
{
    int res = 0;
    int len = (int) strlen(buf);
    int n = ou_min(bufsz - len - 1, (int) strlen(ctx->text_input));
    unused(r); unused(opt);
    if (n > 0) {
        memcpy(buf + len, ctx->text_input, (size_t) n);
        len += n;
        buf[len] = '\0';
        res |= OU_SIGNAL_ALTERED;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }
    if (ctx->key_pressed & OU_KEY_ERASE && len > 0) {
        while ((buf[--len] & 0xc0) == 0x80 && len > 0);
        buf[len] = '\0';
        res |= OU_SIGNAL_ALTERED;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }
    if (ctx->key_pressed & OU_KEY_ENTER) {
        ou_grant_focus(ctx, 0);
        res |= OU_SIGNAL_COMMIT;
        ou_notify(ctx, id, OU_SIGNAL_COMMIT);
    }
    return res;
}

static void render_focused(ou_context *ctx, const char *buf, ou_id id,
                           ou_rect r, int opt)
{
    ou_color color = ctx->theme->colors[OU_TINT_LABEL];
    ou_font font = ctx->theme->font;
    int textw = ctx->text_width(font, buf, -1);
    int texth = ctx->text_height(font);
    int ofx = r.w - ctx->theme->padding - textw - 1;
    int textx = r.x + ou_min(ofx, ctx->theme->padding);
    int texty = r.y + (r.h - texth) / 2;
    ou_decorate(ctx, id, r, OU_TINT_FIELD, opt);
    ou_push_scissor(ctx, r);
    ou_paint_text(ctx, font, buf, -1, ou_vec2_make(textx, texty), color);
    ou_paint_rect(ctx, ou_rect_make(textx + textw, texty, 1, texth), color);
    ou_pop_scissor(ctx);
}


static const ou_tfield_state_vtable tfield_states[] = {
    [OU_TFIELD_IDLE]    = { handle_idle,    render_idle    },
    [OU_TFIELD_FOCUSED] = { handle_focused, render_focused },
};


int ou_textfield_raw(ou_context *ctx, char *buf, int bufsz, ou_id id,
                     ou_rect r, int opt)
{
    int res = 0;
    int state;

    ou_sense(ctx, id, r, opt | OU_HINT_STICKY);

    /* determine current state */
    state = (ctx->focus == id) ? OU_TFIELD_FOCUSED : OU_TFIELD_IDLE;
    res = tfield_states[state].handle(ctx, buf, bufsz, id, r, opt);

    /* re-evaluate state after handling (enter key may have cleared focus) */
    state = (ctx->focus == id) ? OU_TFIELD_FOCUSED : OU_TFIELD_IDLE;
    tfield_states[state].render(ctx, buf, id, r, opt);

    return res;
}

int ou_textfield_ex(ou_context *ctx, char *buf, int bufsz, int opt) {
    ou_id id = ou_hash(ctx, &buf, sizeof(buf));
    ou_rect r = ou_allocate(ctx);
    return ou_textfield_raw(ctx, buf, bufsz, id, r, opt);
}
