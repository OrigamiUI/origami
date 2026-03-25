/*
** Origami UI - Lightweight Immediate-Mode GUI Library
** Copyright (c) 2026 Origami UI Contributors
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to
** deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
** sell copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
*/

#include "origami_internal.h"


ou_rect ou__infinite_rect = { 0, 0, 0x1000000, 0x1000000 };

ou_theme ou__base_theme = {
    /* font | size | padding | spacing | indent */
    NULL, { 68, 10 }, 5, 4, 24,
    /* title_height | scrollbar_size | thumb_size */
    24, 12, 8,
    {
        { 230, 230, 230, 255 }, /* OU_TINT_LABEL */
        {  25,  25,  25, 255 }, /* OU_TINT_EDGE */
        {  50,  50,  50, 255 }, /* OU_TINT_CANVAS */
        {  25,  25,  25, 255 }, /* OU_TINT_HEADER */
        { 240, 240, 240, 255 }, /* OU_TINT_HEADING */
        {   0,   0,   0,   0 }, /* OU_TINT_SURFACE */
        {  75,  75,  75, 255 }, /* OU_TINT_CONTROL */
        {  95,  95,  95, 255 }, /* OU_TINT_CONTROL_WARM */
        { 115, 115, 115, 255 }, /* OU_TINT_CONTROL_HOT */
        {  30,  30,  30, 255 }, /* OU_TINT_FIELD */
        {  35,  35,  35, 255 }, /* OU_TINT_FIELD_WARM */
        {  40,  40,  40, 255 }, /* OU_TINT_FIELD_HOT */
        {  43,  43,  43, 255 }, /* OU_TINT_TRACK */
        {  30,  30,  30, 255 }, /* OU_TINT_HANDLE */
        {  75, 145, 215, 255 }, /* OU_TINT_ACCENT */
        {   0,   0,   0, 128 }  /* OU_TINT_OVERLAY */
    }
};


/*============================================================================
** constructors
**============================================================================*/

ou_vec2 ou_vec2_make(int x, int y) {
    ou_vec2 v;
    v.x = x; v.y = y;
    return v;
}

ou_rect ou_rect_make(int x, int y, int w, int h) {
    ou_rect r;
    r.x = x; r.y = y; r.w = w; r.h = h;
    return r;
}

ou_color ou_color_make(int r, int g, int b, int a) {
    ou_color c;
    c.r = (unsigned char) r;
    c.g = (unsigned char) g;
    c.b = (unsigned char) b;
    c.a = (unsigned char) a;
    return c;
}


/*============================================================================
** internal rect helpers
**============================================================================*/

ou_rect ou__grow_rect(ou_rect r, int n) {
    return ou_rect_make(r.x - n, r.y - n, r.w + n * 2, r.h + n * 2);
}

ou_rect ou__overlap_rects(ou_rect a, ou_rect b) {
    int x1 = ou_max(a.x, b.x);
    int y1 = ou_max(a.y, b.y);
    int x2 = ou_min(a.x + a.w, b.x + b.w);
    int y2 = ou_min(a.y + a.h, b.y + b.h);
    if (x2 < x1) { x2 = x1; }
    if (y2 < y1) { y2 = y1; }
    return ou_rect_make(x1, y1, x2 - x1, y2 - y1);
}

int ou__point_in_rect(ou_rect r, ou_vec2 p) {
    return p.x >= r.x && p.x < r.x + r.w && p.y >= r.y && p.y < r.y + r.h;
}




static void render_frame(ou_context *ctx, ou_rect rect, int colorid) {
    ou_paint_rect(ctx, rect, ctx->theme->colors[colorid]);
    if (colorid == OU_TINT_TRACK  ||
        colorid == OU_TINT_HANDLE ||
        colorid == OU_TINT_HEADER) { return; }
    if (ctx->theme->colors[OU_TINT_EDGE].a) {
        ou_paint_outline(ctx, ou__grow_rect(rect, 1),
                         ctx->theme->colors[OU_TINT_EDGE]);
    }
}


/*============================================================================
** lifecycle
**============================================================================*/

void ou_setup(ou_context *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->draw_frame = render_frame;
    ctx->_theme = ou__base_theme;
    ctx->theme = &ctx->_theme;
}

void ou_frame_begin(ou_context *ctx) {
    verify(ctx->text_width && ctx->text_height);
    ctx->cmd_buf.idx = 0;
    ctx->root_list.idx = 0;
    ctx->scroll_target = NULL;
    ctx->hover_root = ctx->next_hover_root;
    ctx->next_hover_root = NULL;
    ctx->mouse_delta.x = ctx->mouse_pos.x - ctx->prev_mouse_pos.x;
    ctx->mouse_delta.y = ctx->mouse_pos.y - ctx->prev_mouse_pos.y;
    ctx->tick++;
}

static int compare_depth(const void *a, const void *b) {
    return (*(ou_frame**) a)->zindex - (*(ou_frame**) b)->zindex;
}

void ou_frame_end(ou_context *ctx) {
    int i, n;
    /* validate all stacks are balanced */
    verify(ctx->frame_stack.idx   == 0);
    verify(ctx->scissor_stack.idx == 0);
    verify(ctx->scope_stack.idx   == 0);
    verify(ctx->grid_stack.idx    == 0);

    /* apply scroll input */
    if (ctx->scroll_target) {
        ctx->scroll_target->scroll.x += ctx->scroll_delta.x;
        ctx->scroll_target->scroll.y += ctx->scroll_delta.y;
    }

    /* clear focus if the focused widget was not touched this tick */
    if (!ctx->focus_alive) { ctx->focus = 0; }
    ctx->focus_alive = 0;

    /* raise hover root on click */
    if (ctx->mouse_pressed && ctx->next_hover_root &&
        ctx->next_hover_root->zindex < ctx->last_zindex &&
        ctx->next_hover_root->zindex >= 0)
    {
        ou_elevate(ctx, ctx->next_hover_root);
    }

    /* reset per-frame input state */
    ctx->key_pressed = 0;
    ctx->text_input[0] = '\0';
    ctx->mouse_pressed = 0;
    ctx->scroll_delta = ou_vec2_make(0, 0);
    ctx->prev_mouse_pos = ctx->mouse_pos;

    /* sort root containers by depth */
    n = ctx->root_list.idx;
    qsort(ctx->root_list.items, (size_t) n, sizeof(ou_frame*), compare_depth);

    /* wire up skip commands to walk containers in depth order */
    for (i = 0; i < n; i++) {
        ou_frame *frm = ctx->root_list.items[i];
        if (i == 0) {
            ou_cmd *cmd = (ou_cmd*) ctx->cmd_buf.items;
            cmd->skip.dst = (char*) frm->head + sizeof(ou_cmd_skip);
        } else {
            ou_frame *prev = ctx->root_list.items[i - 1];
            prev->tail->skip.dst = (char*) frm->head + sizeof(ou_cmd_skip);
        }
        if (i == n - 1) {
            frm->tail->skip.dst = ctx->cmd_buf.items + ctx->cmd_buf.idx;
        }
    }
}


/*============================================================================
** identity system (FNV-1a 32-bit)
**============================================================================*/

#define SEED_VALUE 2166136261u

static void hash_bytes(ou_id *h, const void *data, int size) {
    const unsigned char *p = data;
    while (size--) {
        *h = (*h ^ *p++) * 16777619u;
    }
}

ou_id ou_hash(ou_context *ctx, const void *data, int size) {
    int idx = ctx->scope_stack.idx;
    ou_id res = (idx > 0) ? ctx->scope_stack.items[idx - 1] : SEED_VALUE;
    hash_bytes(&res, data, size);
    ctx->last_id = res;
    return res;
}

void ou_push_scope(ou_context *ctx, const void *data, int size) {
    push(ctx->scope_stack, ou_hash(ctx, data, size));
}

void ou_pop_scope(ou_context *ctx) {
    pop(ctx->scope_stack);
}


/*============================================================================
** focus
**============================================================================*/

void ou_grant_focus(ou_context *ctx, ou_id id) {
    ctx->focus = id;
    ctx->focus_alive = 1;
}


/*============================================================================
** clipping
**============================================================================*/

void ou_push_scissor(ou_context *ctx, ou_rect rect) {
    ou_rect last = ou_current_scissor(ctx);
    push(ctx->scissor_stack, ou__overlap_rects(rect, last));
}

void ou_pop_scissor(ou_context *ctx) {
    pop(ctx->scissor_stack);
}

ou_rect ou_current_scissor(ou_context *ctx) {
    verify(ctx->scissor_stack.idx > 0);
    return ctx->scissor_stack.items[ctx->scissor_stack.idx - 1];
}

int ou_test_scissor(ou_context *ctx, ou_rect r) {
    ou_rect cr = ou_current_scissor(ctx);
    if (r.x > cr.x + cr.w || r.x + r.w < cr.x ||
        r.y > cr.y + cr.h || r.y + r.h < cr.y) { return OU_CLIP_TOTAL; }
    if (r.x >= cr.x && r.x + r.w <= cr.x + cr.w &&
        r.y >= cr.y && r.y + r.h <= cr.y + cr.h) { return 0; }
    return OU_CLIP_PARTIAL;
}


/*============================================================================
** layout internals
**============================================================================*/

void ou__push_grid(ou_context *ctx, ou_rect body, ou_vec2 scroll) {
    ou_grid g;
    int width = 0;
    memset(&g, 0, sizeof(g));
    g.body = ou_rect_make(body.x - scroll.x, body.y - scroll.y,
                          body.w, body.h);
    g.max = ou_vec2_make(-0x1000000, -0x1000000);
    push(ctx->grid_stack, g);
    ou_row(ctx, 1, &width, 0);
}

ou_grid *ou__current_grid(ou_context *ctx) {
    return &ctx->grid_stack.items[ctx->grid_stack.idx - 1];
}

void ou__pop_frame_state(ou_context *ctx) {
    ou_frame *frm = ou_active_frame(ctx);
    ou_grid *g = ou__current_grid(ctx);
    frm->content_size.x = g->max.x - g->body.x;
    frm->content_size.y = g->max.y - g->body.y;
    pop(ctx->frame_stack);
    pop(ctx->grid_stack);
    ou_pop_scope(ctx);
}


/*============================================================================
** frame / container management
**============================================================================*/

ou_frame *ou_active_frame(ou_context *ctx) {
    verify(ctx->frame_stack.idx > 0);
    return ctx->frame_stack.items[ctx->frame_stack.idx - 1];
}

ou_frame *ou__resolve_frame(ou_context *ctx, ou_id id, int opt) {
    ou_frame *frm;
    int idx = ou_cache_find(ctx, ctx->frame_pool, OU_FRAME_POOL_CAP, id);
    if (idx >= 0) {
        if (ctx->frames[idx].open || ~opt & OU_HINT_HIDDEN) {
            ou_cache_touch(ctx, ctx->frame_pool, idx);
        }
        return &ctx->frames[idx];
    }
    if (opt & OU_HINT_HIDDEN) { return NULL; }
    idx = ou_cache_alloc(ctx, ctx->frame_pool, OU_FRAME_POOL_CAP, id);
    frm = &ctx->frames[idx];
    memset(frm, 0, sizeof(*frm));
    frm->open = 1;
    ou_elevate(ctx, frm);
    return frm;
}

ou_frame *ou_find_frame(ou_context *ctx, const char *name) {
    ou_id id = ou_hash(ctx, name, (int) strlen(name));
    return ou__resolve_frame(ctx, id, 0);
}

void ou_elevate(ou_context *ctx, ou_frame *f) {
    f->zindex = ++ctx->last_zindex;
}




int ou_cache_alloc(ou_context *ctx, ou_cache_slot *items, int len, ou_id id) {
    int i, n = -1, f = ctx->tick;
    for (i = 0; i < len; i++) {
        if (items[i].last_touch < f) {
            f = items[i].last_touch;
            n = i;
        }
    }
    verify(n > -1);
    items[n].id = id;
    ou_cache_touch(ctx, items, n);
    return n;
}

int ou_cache_find(ou_context *ctx, ou_cache_slot *items, int len, ou_id id) {
    int i;
    unused(ctx);
    for (i = 0; i < len; i++) {
        if (items[i].id == id) { return i; }
    }
    return -1;
}

void ou_cache_touch(ou_context *ctx, ou_cache_slot *items, int idx) {
    items[idx].last_touch = ctx->tick;
}


/*============================================================================
** input feeding
**============================================================================*/

void ou_feed_motion(ou_context *ctx, int x, int y) {
    ctx->mouse_pos = ou_vec2_make(x, y);
}

void ou_feed_press(ou_context *ctx, int x, int y, int btn) {
    ou_feed_motion(ctx, x, y);
    ctx->mouse_down |= btn;
    ctx->mouse_pressed |= btn;
}

void ou_feed_release(ou_context *ctx, int x, int y, int btn) {
    ou_feed_motion(ctx, x, y);
    ctx->mouse_down &= ~btn;
}

void ou_feed_scroll(ou_context *ctx, int x, int y) {
    ctx->scroll_delta.x += x;
    ctx->scroll_delta.y += y;
}

void ou_feed_keydown(ou_context *ctx, int key) {
    ctx->key_pressed |= key;
    ctx->key_down |= key;
}

void ou_feed_keyup(ou_context *ctx, int key) {
    ctx->key_down &= ~key;
}

void ou_feed_text(ou_context *ctx, const char *text) {
    int len = (int) strlen(ctx->text_input);
    int n = (int) strlen(text) + 1;
    verify(len + n <= (int) sizeof(ctx->text_input));
    memcpy(ctx->text_input + len, text, (size_t) n);
}



ou_cmd *ou_emit(ou_context *ctx, int type, int size) {
    ou_cmd *cmd = (ou_cmd*) (ctx->cmd_buf.items + ctx->cmd_buf.idx);
    verify(ctx->cmd_buf.idx + size < OU_CMD_BUF_SIZE);
    cmd->base.type = type;
    cmd->base.size = size;
    ctx->cmd_buf.idx += size;
    return cmd;
}



int ou_walk(ou_context *ctx, ou_cmd **cmd) {
    if (*cmd) {
        *cmd = (ou_cmd*) (((char*) *cmd) + (*cmd)->base.size);
    } else {
        *cmd = (ou_cmd*) ctx->cmd_buf.items;
    }
    while ((char*) *cmd != ctx->cmd_buf.items + ctx->cmd_buf.idx) {
        if ((*cmd)->type != OU_CMD_SKIP) { return 1; }
        *cmd = (*cmd)->skip.dst;
    }
    return 0;
}

ou_cmd *ou__emit_skip(ou_context *ctx, ou_cmd *dst) {
    ou_cmd *cmd;
    cmd = ou_emit(ctx, OU_CMD_SKIP, sizeof(ou_cmd_skip));
    cmd->skip.dst = dst;
    return cmd;
}


/*============================================================================
** drawing primitives
**============================================================================*/

void ou_clip(ou_context *ctx, ou_rect rect) {
    ou_cmd *cmd;
    cmd = ou_emit(ctx, OU_CMD_SCISSOR, sizeof(ou_cmd_scissor));
    cmd->scissor.rect = rect;
}

void ou_paint_rect(ou_context *ctx, ou_rect rect, ou_color color) {
    ou_cmd *cmd;
    rect = ou__overlap_rects(rect, ou_current_scissor(ctx));
    if (rect.w > 0 && rect.h > 0) {
        cmd = ou_emit(ctx, OU_CMD_FILL, sizeof(ou_cmd_fill));
        cmd->fill.rect = rect;
        cmd->fill.color = color;
    }
}

void ou_paint_outline(ou_context *ctx, ou_rect rect, ou_color color) {
    ou_paint_rect(ctx, ou_rect_make(rect.x + 1, rect.y, rect.w - 2, 1), color);
    ou_paint_rect(ctx, ou_rect_make(rect.x + 1, rect.y + rect.h - 1,
                                    rect.w - 2, 1), color);
    ou_paint_rect(ctx, ou_rect_make(rect.x, rect.y, 1, rect.h), color);
    ou_paint_rect(ctx, ou_rect_make(rect.x + rect.w - 1, rect.y, 1, rect.h),
                  color);
}

void ou_paint_text(ou_context *ctx, ou_font font, const char *str, int len,
                   ou_vec2 pos, ou_color color)
{
    ou_cmd *cmd;
    ou_rect rect = ou_rect_make(
        pos.x, pos.y, ctx->text_width(font, str, len),
        ctx->text_height(font));
    int clipped = ou_test_scissor(ctx, rect);
    if (clipped == OU_CLIP_TOTAL) { return; }
    if (clipped == OU_CLIP_PARTIAL) {
        ou_clip(ctx, ou_current_scissor(ctx));
    }
    if (len < 0) { len = (int) strlen(str); }
    cmd = ou_emit(ctx, OU_CMD_GLYPH, (int) sizeof(ou_cmd_glyph) + len);
    memcpy(cmd->glyph.str, str, (size_t) len);
    cmd->glyph.str[len] = '\0';
    cmd->glyph.pos = pos;
    cmd->glyph.color = color;
    cmd->glyph.font = font;
    if (clipped) { ou_clip(ctx, ou__infinite_rect); }
}

void ou_paint_symbol(ou_context *ctx, int id, ou_rect rect, ou_color color) {
    ou_cmd *cmd;
    int clipped = ou_test_scissor(ctx, rect);
    if (clipped == OU_CLIP_TOTAL) { return; }
    if (clipped == OU_CLIP_PARTIAL) {
        ou_clip(ctx, ou_current_scissor(ctx));
    }
    cmd = ou_emit(ctx, OU_CMD_SYMBOL, sizeof(ou_cmd_symbol));
    cmd->symbol.id = id;
    cmd->symbol.rect = rect;
    cmd->symbol.color = color;
    if (clipped) { ou_clip(ctx, ou__infinite_rect); }
}


/*============================================================================
** layout
**============================================================================*/

enum { LAYOUT_RELATIVE = 1, LAYOUT_ABSOLUTE = 2 };

void ou_row(ou_context *ctx, int items, const int *widths, int height) {
    ou_grid *g = ou__current_grid(ctx);
    if (widths) {
        verify(items <= OU_MAX_COLUMNS);
        memcpy(g->widths, widths, (size_t) items * sizeof(widths[0]));
    }
    g->items = items;
    g->position = ou_vec2_make(g->indent, g->next_row);
    g->size.y = height;
    g->item_index = 0;
}

void ou_set_width(ou_context *ctx, int width) {
    ou__current_grid(ctx)->size.x = width;
}

void ou_set_height(ou_context *ctx, int height) {
    ou__current_grid(ctx)->size.y = height;
}

void ou_place(ou_context *ctx, ou_rect r, int relative) {
    ou_grid *g = ou__current_grid(ctx);
    g->next = r;
    g->next_type = relative ? LAYOUT_RELATIVE : LAYOUT_ABSOLUTE;
}

ou_rect ou_allocate(ou_context *ctx) {
    ou_grid *g = ou__current_grid(ctx);
    ou_theme *th = ctx->theme;
    ou_rect res;

    if (g->next_type) {
        int type = g->next_type;
        g->next_type = 0;
        res = g->next;
        if (type == LAYOUT_ABSOLUTE) { return (ctx->last_rect = res); }

    } else {
        if (g->item_index == g->items) {
            ou_row(ctx, g->items, NULL, g->size.y);
        }

        res.x = g->position.x;
        res.y = g->position.y;

        res.w = g->items > 0 ? g->widths[g->item_index] : g->size.x;
        res.h = g->size.y;
        if (res.w == 0) { res.w = th->size.x + th->padding * 2; }
        if (res.h == 0) { res.h = th->size.y + th->padding * 2; }
        if (res.w <  0) { res.w += g->body.w - res.x + 1; }
        if (res.h <  0) { res.h += g->body.h - res.y + 1; }

        g->item_index++;
    }

    g->position.x += res.w + th->spacing;
    g->next_row = ou_max(g->next_row, res.y + res.h + th->spacing);

    res.x += g->body.x;
    res.y += g->body.y;

    g->max.x = ou_max(g->max.x, res.x + res.w);
    g->max.y = ou_max(g->max.y, res.y + res.h);

    return (ctx->last_rect = res);
}

void ou_column_begin(ou_context *ctx) {
    ou__push_grid(ctx, ou_allocate(ctx), ou_vec2_make(0, 0));
}

void ou_column_end(ou_context *ctx) {
    ou_grid *child, *parent;
    child = ou__current_grid(ctx);
    pop(ctx->grid_stack);
    parent = ou__current_grid(ctx);
    parent->position.x = ou_max(parent->position.x,
        child->position.x + child->body.x - parent->body.x);
    parent->next_row = ou_max(parent->next_row,
        child->next_row + child->body.y - parent->body.y);
    parent->max.x = ou_max(parent->max.x, child->max.x);
    parent->max.y = ou_max(parent->max.y, child->max.y);
}


/*============================================================================
** control helpers
**============================================================================*/

int ou__in_hover_root(ou_context *ctx) {
    int i = ctx->frame_stack.idx;
    while (i--) {
        if (ctx->frame_stack.items[i] == ctx->hover_root) { return 1; }
        if (ctx->frame_stack.items[i]->head) { break; }
    }
    return 0;
}

void ou_decorate(ou_context *ctx, ou_id id, ou_rect rect, int colorid,
                 int opt)
{
    if (opt & OU_HINT_BARE) { return; }
    colorid += (ctx->focus == id) ? 2 : (ctx->hover == id) ? 1 : 0;
    ctx->draw_frame(ctx, rect, colorid);
}

void ou_inscribe(ou_context *ctx, const char *str, ou_rect rect, int colorid,
                 int opt)
{
    ou_vec2 pos;
    ou_font font = ctx->theme->font;
    int tw = ctx->text_width(font, str, -1);
    ou_push_scissor(ctx, rect);
    pos.y = rect.y + (rect.h - ctx->text_height(font)) / 2;
    if (opt & OU_HINT_CENTER) {
        pos.x = rect.x + (rect.w - tw) / 2;
    } else if (opt & OU_HINT_RIGHT) {
        pos.x = rect.x + rect.w - tw - ctx->theme->padding;
    } else {
        pos.x = rect.x + ctx->theme->padding;
    }
    ou_paint_text(ctx, font, str, -1, pos, ctx->theme->colors[colorid]);
    ou_pop_scissor(ctx);
}

int ou_hovering(ou_context *ctx, ou_rect rect) {
    return ou__point_in_rect(rect, ctx->mouse_pos) &&
           ou__point_in_rect(ou_current_scissor(ctx), ctx->mouse_pos) &&
           ou__in_hover_root(ctx);
}

void ou_sense(ou_context *ctx, ou_id id, ou_rect rect, int opt) {
    int over = ou_hovering(ctx, rect);

    if (ctx->focus == id) { ctx->focus_alive = 1; }
    if (opt & OU_HINT_INERT) { return; }
    if (over && !ctx->mouse_down) { ctx->hover = id; }

    if (ctx->focus == id) {
        if (ctx->mouse_pressed && !over) { ou_grant_focus(ctx, 0); }
        if (!ctx->mouse_down && ~opt & OU_HINT_STICKY) {
            ou_grant_focus(ctx, 0);
        }
    }

    if (ctx->hover == id) {
        if (ctx->mouse_pressed) {
            ou_grant_focus(ctx, id);
        } else if (!over) {
            ctx->hover = 0;
        }
    }
}


/*============================================================================
** scrollbars
**============================================================================*/

#define render_scrollbar(ctx, frm, b, cs, x, y, w, h)                       \
    do {                                                                    \
        int maxscroll = cs.y - b->h;                                        \
        if (maxscroll > 0 && b->h > 0) {                                    \
            ou_rect base, thumb;                                            \
            ou_id id = ou_hash(ctx, "!scroll" #y, 8);                       \
            base = *b;                                                      \
            base.x = b->x + b->w;                                           \
            base.w = ctx->theme->scrollbar_size;                            \
            ou_sense(ctx, id, base, 0);                                     \
            if (ctx->focus == id && ctx->mouse_down == OU_MOUSE_PRIMARY) {  \
                frm->scroll.y += ctx->mouse_delta.y * cs.y / base.h;        \
            }                                                               \
            frm->scroll.y = ou_clamp(frm->scroll.y, 0, maxscroll);          \
            ctx->draw_frame(ctx, base, OU_TINT_TRACK);                      \
            thumb = base;                                                   \
            thumb.h = ou_max(ctx->theme->thumb_size, base.h * b->h / cs.y); \
            thumb.y += frm->scroll.y * (base.h - thumb.h) / maxscroll;      \
            ctx->draw_frame(ctx, thumb, OU_TINT_HANDLE);                    \
            if (ou_hovering(ctx, *b)) { ctx->scroll_target = frm; }         \
        } else {                                                            \
            frm->scroll.y = 0;                                              \
        }                                                                   \
    } while (0)

void ou__handle_scrollbars(ou_context *ctx, ou_frame *frm, ou_rect *body) {
    int sz = ctx->theme->scrollbar_size;
    ou_vec2 cs = frm->content_size;
    cs.x += ctx->theme->padding * 2;
    cs.y += ctx->theme->padding * 2;
    ou_push_scissor(ctx, *body);
    if (cs.y > frm->body.h) { body->w -= sz; }
    if (cs.x > frm->body.w) { body->h -= sz; }
    render_scrollbar(ctx, frm, body, cs, x, y, w, h);
    render_scrollbar(ctx, frm, body, cs, y, x, h, w);
    ou_pop_scissor(ctx);
}

void ou__push_frame_body(ou_context *ctx, ou_frame *frm, ou_rect body,
                         int opt)
{
    if (~opt & OU_HINT_FIXED) { ou__handle_scrollbars(ctx, frm, &body); }
    ou__push_grid(ctx, ou__grow_rect(body, -ctx->theme->padding), frm->scroll);
    frm->body = body;
}


/*============================================================================
** root container lifecycle
**============================================================================*/

void ou__begin_root(ou_context *ctx, ou_frame *frm) {
    push(ctx->frame_stack, frm);
    push(ctx->root_list, frm);
    frm->head = ou__emit_skip(ctx, NULL);
    if (ou__point_in_rect(frm->rect, ctx->mouse_pos) &&
        (!ctx->next_hover_root ||
         frm->zindex > ctx->next_hover_root->zindex))
    {
        ctx->next_hover_root = frm;
    }
    push(ctx->scissor_stack, ou__infinite_rect);
}

void ou__end_root(ou_context *ctx) {
    ou_frame *frm = ou_active_frame(ctx);
    frm->tail = ou__emit_skip(ctx, NULL);
    frm->head->skip.dst = ctx->cmd_buf.items + ctx->cmd_buf.idx;
    ou_pop_scissor(ctx);
    ou__pop_frame_state(ctx);
}


/*============================================================================
** shared widget helpers
**============================================================================*/

int ou__collapsible(ou_context *ctx, const char *label, int is_tree, int opt) {
    ou_rect r;
    int active, expanded;
    ou_id id = ou_hash(ctx, label, (int) strlen(label));
    int idx = ou_cache_find(ctx, ctx->tree_pool, OU_TREE_POOL_CAP, id);
    int width = -1;
    ou_row(ctx, 1, &width, 0);

    active = (idx >= 0);
    expanded = (opt & OU_HINT_UNFOLDED) ? !active : active;
    r = ou_allocate(ctx);
    ou_sense(ctx, id, r, 0);

    active ^= (ctx->mouse_pressed == OU_MOUSE_PRIMARY && ctx->focus == id);

    if (idx >= 0) {
        if (active) { ou_cache_touch(ctx, ctx->tree_pool, idx); }
        else { memset(&ctx->tree_pool[idx], 0, sizeof(ou_cache_slot)); }
    } else if (active) {
        ou_cache_alloc(ctx, ctx->tree_pool, OU_TREE_POOL_CAP, id);
    }

    if (is_tree) {
        if (ctx->hover == id) {
            ctx->draw_frame(ctx, r, OU_TINT_CONTROL_WARM);
        }
    } else {
        ou_decorate(ctx, id, r, OU_TINT_CONTROL, 0);
    }
    ou_paint_symbol(
        ctx, expanded ? OU_GLYPH_UNFOLDED : OU_GLYPH_FOLDED,
        ou_rect_make(r.x, r.y, r.h, r.h),
        ctx->theme->colors[OU_TINT_LABEL]);
    r.x += r.h - ctx->theme->padding;
    r.w -= r.h - ctx->theme->padding;
    ou_inscribe(ctx, label, r, OU_TINT_LABEL, 0);

    return expanded ? OU_SIGNAL_ACTIVE : 0;
}

/*============================================================================
** observer
**============================================================================*/

void ou_observe(ou_context *ctx, ou_id source, int signal_mask,
                ou_observer_fn cb, void *ud)
{
    if (ctx->observer_count >= OU_MAX_OBSERVERS) { return; }
    {
        ou_observer *o = &ctx->observers[ctx->observer_count++];
        o->source      = source;
        o->signal_mask = signal_mask;
        o->callback    = cb;
        o->userdata    = ud;
    }
}

void ou_unobserve(ou_context *ctx, ou_id source, ou_observer_fn cb) {
    int i;
    for (i = 0; i < ctx->observer_count; i++) {
        if (ctx->observers[i].source == source &&
            ctx->observers[i].callback == cb)
        {
            ctx->observers[i] = ctx->observers[--ctx->observer_count];
            return;
        }
    }
}

void ou_notify(ou_context *ctx, ou_id source, int signal) {
    int i;
    for (i = 0; i < ctx->observer_count; i++) {
        ou_observer *o = &ctx->observers[i];
        if (o->source == source && (o->signal_mask & signal)) {
            o->callback(ctx, source, signal, o->userdata);
        }
    }
}


int ou__number_textfield(ou_context *ctx, ou_real *value, ou_rect r,
                         ou_id id)
{
    if (ctx->mouse_pressed == OU_MOUSE_PRIMARY &&
        ctx->key_down & OU_KEY_SHIFT && ctx->hover == id)
    {
        ctx->number_edit = id;
        sprintf(ctx->number_edit_buf, OU_REAL_FMT, *value);
    }
    if (ctx->number_edit == id) {
        int res = ou_textfield_raw(
            ctx, ctx->number_edit_buf,
            (int) sizeof(ctx->number_edit_buf), id, r, 0);
        if (res & OU_SIGNAL_COMMIT || ctx->focus != id) {
            *value = (ou_real) strtod(ctx->number_edit_buf, NULL);
            ctx->number_edit = 0;
        } else {
            return 1;
        }
    }
    return 0;
}
