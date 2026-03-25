/*
** Origami UI - Lightweight Immediate-Mode GUI Library
** Copyright (c) 2026 Origami UI Contributors
** MIT License - See origami.c for details.
*/

#ifndef ORIGAMI_H
#define ORIGAMI_H

#define OU_VERSION "1.0"

#define OU_CMD_BUF_SIZE     (256 * 1024)
#define OU_ROOT_LIMIT       32
#define OU_FRAME_DEPTH      32
#define OU_SCISSOR_DEPTH    32
#define OU_SCOPE_DEPTH      32
#define OU_GRID_DEPTH       16
#define OU_FRAME_POOL_CAP   48
#define OU_TREE_POOL_CAP    48
#define OU_MAX_COLUMNS      16
#define OU_REAL             float
#define OU_REAL_FMT         "%.3g"
#define OU_SLIDER_FMT       "%.2f"
#define OU_FMT_LIMIT        127

#define ou_stack(T, n)      struct { int idx; T items[n]; }
#define ou_min(a, b)        ((a) < (b) ? (a) : (b))
#define ou_max(a, b)        ((a) > (b) ? (a) : (b))
#define ou_clamp(x, a, b)   ou_min(b, ou_max(a, x))

enum {
    OU_CLIP_PARTIAL = 1,
    OU_CLIP_TOTAL
};

enum {
    OU_CMD_SKIP = 1,
    OU_CMD_SCISSOR,
    OU_CMD_FILL,
    OU_CMD_GLYPH,
    OU_CMD_SYMBOL,
    OU_CMD_COUNT
};

enum {
    OU_TINT_LABEL,
    OU_TINT_EDGE,
    OU_TINT_CANVAS,
    OU_TINT_HEADER,
    OU_TINT_HEADING,
    OU_TINT_SURFACE,
    OU_TINT_CONTROL,
    OU_TINT_CONTROL_WARM,
    OU_TINT_CONTROL_HOT,
    OU_TINT_FIELD,
    OU_TINT_FIELD_WARM,
    OU_TINT_FIELD_HOT,
    OU_TINT_TRACK,
    OU_TINT_HANDLE,
    OU_TINT_ACCENT,
    OU_TINT_OVERLAY,
    OU_TINT_COUNT
};

enum {
    OU_GLYPH_DISMISS = 1,
    OU_GLYPH_TICK,
    OU_GLYPH_FOLDED,
    OU_GLYPH_UNFOLDED,
    OU_GLYPH_COUNT
};

enum {
    OU_SIGNAL_ACTIVE  = (1 << 0),
    OU_SIGNAL_COMMIT  = (1 << 1),
    OU_SIGNAL_ALTERED = (1 << 2)
};

enum {
    OU_HINT_CENTER    = (1 << 0),
    OU_HINT_RIGHT     = (1 << 1),
    OU_HINT_INERT     = (1 << 2),
    OU_HINT_BARE      = (1 << 3),
    OU_HINT_RIGID     = (1 << 4),
    OU_HINT_FIXED     = (1 << 5),
    OU_HINT_PERMANENT = (1 << 6),
    OU_HINT_HEADLESS  = (1 << 7),
    OU_HINT_STICKY    = (1 << 8),
    OU_HINT_SHRINK    = (1 << 9),
    OU_HINT_OVERLAY   = (1 << 10),
    OU_HINT_HIDDEN    = (1 << 11),
    OU_HINT_UNFOLDED  = (1 << 12)
};

enum {
    OU_MOUSE_PRIMARY   = (1 << 0),
    OU_MOUSE_SECONDARY = (1 << 1),
    OU_MOUSE_TERTIARY  = (1 << 2)
};

enum {
    OU_KEY_SHIFT = (1 << 0),
    OU_KEY_CTRL  = (1 << 1),
    OU_KEY_ALT   = (1 << 2),
    OU_KEY_ERASE = (1 << 3),
    OU_KEY_ENTER = (1 << 4)
};


typedef struct ou_context ou_context;
typedef unsigned ou_id;
typedef OU_REAL ou_real;
typedef void *ou_font;

typedef struct { int x, y; } ou_vec2;
typedef struct { int x, y, w, h; } ou_rect;
typedef struct { unsigned char r, g, b, a; } ou_color;
typedef struct { ou_id id; int last_touch; } ou_cache_slot;

typedef struct { int type, size; } ou_cmd_base;
typedef struct { ou_cmd_base base; void *dst; } ou_cmd_skip;
typedef struct { ou_cmd_base base; ou_rect rect; } ou_cmd_scissor;
typedef struct { ou_cmd_base base; ou_rect rect; ou_color color; } ou_cmd_fill;
typedef struct { ou_cmd_base base; ou_font font; ou_vec2 pos; ou_color color; char str[1]; } ou_cmd_glyph;
typedef struct { ou_cmd_base base; ou_rect rect; int id; ou_color color; } ou_cmd_symbol;

typedef union {
    int type;
    ou_cmd_base base;
    ou_cmd_skip skip;
    ou_cmd_scissor scissor;
    ou_cmd_fill fill;
    ou_cmd_glyph glyph;
    ou_cmd_symbol symbol;
} ou_cmd;

typedef struct {
    ou_rect body;
    ou_rect next;
    ou_vec2 position;
    ou_vec2 size;
    ou_vec2 max;
    int widths[OU_MAX_COLUMNS];
    int items;
    int item_index;
    int next_row;
    int next_type;
    int indent;
} ou_grid;

typedef struct {
    ou_cmd *head, *tail;
    ou_rect rect;
    ou_rect body;
    ou_vec2 content_size;
    ou_vec2 scroll;
    int zindex;
    int open;
} ou_frame;

typedef struct {
    ou_font font;
    ou_vec2 size;
    int padding;
    int spacing;
    int indent;
    int title_height;
    int scrollbar_size;
    int thumb_size;
    ou_color colors[OU_TINT_COUNT];
} ou_theme;




#define OU_MAX_OBSERVERS 32

typedef void (*ou_observer_fn)(ou_context *ctx, ou_id source, int signal,
                               void *userdata);

typedef struct {
    ou_id          source;
    int            signal_mask;
    ou_observer_fn callback;
    void          *userdata;
} ou_observer;



typedef struct {
    ou_theme theme;
} ou_theme_builder;


/*============================================================================
** context
**============================================================================*/

struct ou_context {
    /* callbacks */
    int  (*text_width)(ou_font font, const char *str, int len);
    int  (*text_height)(ou_font font);
    void (*draw_frame)(ou_context *ctx, ou_rect rect, int colorid);
    /* core state */
    ou_theme  _theme;
    ou_theme *theme;
    ou_id     hover;
    ou_id     focus;
    ou_id     last_id;
    ou_rect   last_rect;
    ou_rect   last_hover_rect;
    int       last_zindex;
    int       focus_alive;
    int       tick;
    ou_frame *hover_root;
    ou_frame *next_hover_root;
    ou_frame *scroll_target;
    char      number_edit_buf[OU_FMT_LIMIT];
    ou_id     number_edit;
    /* stacks */
    ou_stack(char,      OU_CMD_BUF_SIZE)  cmd_buf;
    ou_stack(ou_frame*, OU_ROOT_LIMIT)    root_list;
    ou_stack(ou_frame*, OU_FRAME_DEPTH)   frame_stack;
    ou_stack(ou_rect,   OU_SCISSOR_DEPTH) scissor_stack;
    ou_stack(ou_id,     OU_SCOPE_DEPTH)   scope_stack;
    ou_stack(ou_grid,   OU_GRID_DEPTH)    grid_stack;
    /* retained state pools */
    ou_cache_slot frame_pool[OU_FRAME_POOL_CAP];
    ou_frame      frames[OU_FRAME_POOL_CAP];
    ou_cache_slot tree_pool[OU_TREE_POOL_CAP];
    /* input state */
    ou_vec2 mouse_pos;
    ou_vec2 prev_mouse_pos;
    ou_vec2 mouse_delta;
    ou_vec2 scroll_delta;
    int     mouse_down;
    int     mouse_pressed;
    int     key_down;
    int     key_pressed;
    char    text_input[32];
    /* observer pattern */
    ou_observer observers[OU_MAX_OBSERVERS];
    int         observer_count;
};


/* constructors */
ou_vec2  ou_vec2_make(int x, int y);
ou_rect  ou_rect_make(int x, int y, int w, int h);
ou_color ou_color_make(int r, int g, int b, int a);

/* lifecycle */
void ou_setup(ou_context *ctx);
void ou_frame_begin(ou_context *ctx);
void ou_frame_end(ou_context *ctx);

/* identity */
ou_id ou_hash(ou_context *ctx, const void *data, int size);
void  ou_push_scope(ou_context *ctx, const void *data, int size);
void  ou_pop_scope(ou_context *ctx);

/* focus */
void ou_grant_focus(ou_context *ctx, ou_id id);

/* clipping */
void    ou_push_scissor(ou_context *ctx, ou_rect rect);
void    ou_pop_scissor(ou_context *ctx);
ou_rect ou_current_scissor(ou_context *ctx);
int     ou_test_scissor(ou_context *ctx, ou_rect r);

/* frame / container management */
ou_frame *ou_active_frame(ou_context *ctx);
ou_frame *ou_find_frame(ou_context *ctx, const char *name);
void      ou_elevate(ou_context *ctx, ou_frame *f);

/* cache / pool */
int  ou_cache_alloc(ou_context *ctx, ou_cache_slot *items, int len, ou_id id);
int  ou_cache_find(ou_context *ctx, ou_cache_slot *items, int len, ou_id id);
void ou_cache_touch(ou_context *ctx, ou_cache_slot *items, int idx);

/* input feeding */
void ou_feed_motion(ou_context *ctx, int x, int y);
void ou_feed_press(ou_context *ctx, int x, int y, int btn);
void ou_feed_release(ou_context *ctx, int x, int y, int btn);
void ou_feed_scroll(ou_context *ctx, int x, int y);
void ou_feed_keydown(ou_context *ctx, int key);
void ou_feed_keyup(ou_context *ctx, int key);
void ou_feed_text(ou_context *ctx, const char *text);

/* command buffer */
ou_cmd *ou_emit(ou_context *ctx, int type, int size);
int     ou_walk(ou_context *ctx, ou_cmd **cmd);

/* drawing primitives */
void ou_clip(ou_context *ctx, ou_rect rect);
void ou_paint_rect(ou_context *ctx, ou_rect rect, ou_color color);
void ou_paint_outline(ou_context *ctx, ou_rect rect, ou_color color);
void ou_paint_text(ou_context *ctx, ou_font font, const char *str, int len,
                   ou_vec2 pos, ou_color color);
void ou_paint_symbol(ou_context *ctx, int id, ou_rect rect, ou_color color);

/* layout */
void    ou_row(ou_context *ctx, int items, const int *widths, int height);
void    ou_set_width(ou_context *ctx, int width);
void    ou_set_height(ou_context *ctx, int height);
void    ou_place(ou_context *ctx, ou_rect r, int relative);
ou_rect ou_allocate(ou_context *ctx);
void    ou_column_begin(ou_context *ctx);
void    ou_column_end(ou_context *ctx);

/* control helpers */
void ou_decorate(ou_context *ctx, ou_id id, ou_rect rect, int colorid, int opt);
void ou_inscribe(ou_context *ctx, const char *str, ou_rect rect, int colorid,
                 int opt);
int  ou_hovering(ou_context *ctx, ou_rect rect);
void ou_sense(ou_context *ctx, ou_id id, ou_rect rect, int opt);

/* widgets */
void ou_paragraph(ou_context *ctx, const char *text);
void ou_label(ou_context *ctx, const char *text);
int  ou_button_ex(ou_context *ctx, const char *label, int icon, int opt);
int  ou_checkbox(ou_context *ctx, const char *label, int *state);
int  ou_textfield_raw(ou_context *ctx, char *buf, int bufsz, ou_id id,
                      ou_rect r, int opt);
int  ou_textfield_ex(ou_context *ctx, char *buf, int bufsz, int opt);
int  ou_slider_ex(ou_context *ctx, ou_real *value, ou_real low, ou_real high,
                  ou_real step, const char *fmt, int opt);
int  ou_spinner_ex(ou_context *ctx, ou_real *value, ou_real step,
                   const char *fmt, int opt);
int  ou_section_ex(ou_context *ctx, const char *label, int opt);
int  ou_tree_begin_ex(ou_context *ctx, const char *label, int opt);
void ou_tree_end(ou_context *ctx);

/* new widgets */
void ou_separator(ou_context *ctx);
int  ou_toggle_ex(ou_context *ctx, const char *label, int *state, int opt);
int  ou_switch_ex(ou_context *ctx, const char *label, int *state, int opt);
void ou_progress_ex(ou_context *ctx, ou_real value, int opt);
void ou_meter_ex(ou_context *ctx, ou_real value, ou_real low, ou_real high,
                 int opt);
void ou_avatar_ex(ou_context *ctx, const char *initials, ou_color bg, int opt);
int  ou_radio(ou_context *ctx, const char *label, int *selected, int value);
void ou_checkbox_group_begin(ou_context *ctx, const char *label);
void ou_checkbox_group_end(ou_context *ctx);
int  ou_toggle_group(ou_context *ctx, const char **labels, int count,
                     int *selected);
int  ou_field_ex(ou_context *ctx, const char *label, char *buf, int bufsz,
                 int opt);
void ou_fieldset_begin(ou_context *ctx, const char *title);
void ou_fieldset_end(ou_context *ctx);
void ou_form_begin(ou_context *ctx, const char *name);
int  ou_form_end(ou_context *ctx);
int  ou_accordion_item(ou_context *ctx, const char *label, int *active,
                       int index);
int  ou_collapsible_begin_ex(ou_context *ctx, const char *label, int opt);
void ou_collapsible_end(ou_context *ctx);
int  ou_tabs(ou_context *ctx, const char **labels, int count, int *selected);
void ou_scroll_area_begin_ex(ou_context *ctx, const char *name, int height,
                             int opt);
void ou_scroll_area_end(ou_context *ctx);
void ou_toolbar_begin(ou_context *ctx);
void ou_toolbar_end(ou_context *ctx);

/* windows */
int  ou_window_begin_ex(ou_context *ctx, const char *title, ou_rect rect,
                        int opt);
void ou_window_end(ou_context *ctx);

/* popups */
void ou_summon_popup(ou_context *ctx, const char *name);
int  ou_popup_begin(ou_context *ctx, const char *name);
void ou_popup_end(ou_context *ctx);

/* dialog */
int  ou_dialog_begin(ou_context *ctx, const char *title, int *open);
void ou_dialog_end(ou_context *ctx);

/* alert dialog */
int  ou_alert_dialog(ou_context *ctx, const char *title, const char *message,
                     int *open);

/* popover */
void ou_summon_popover(ou_context *ctx, const char *name);
int  ou_popover_begin(ou_context *ctx, const char *name);
void ou_popover_end(ou_context *ctx);

/* tooltip */
void ou_tooltip(ou_context *ctx, const char *text);

/* context menu */
void ou_open_context_menu(ou_context *ctx, const char *name);
int  ou_context_menu_begin(ou_context *ctx, const char *name);
void ou_context_menu_end(ou_context *ctx);

/* select */
int  ou_select(ou_context *ctx, const char **items, int count, int *selected);

/* preview card */
int  ou_preview_card_begin(ou_context *ctx, const char *name);
void ou_preview_card_end(ou_context *ctx);

/* panels */
void ou_panel_begin_ex(ou_context *ctx, const char *name, int opt);
void ou_panel_end(ou_context *ctx);

/* observer pattern */
void ou_observe(ou_context *ctx, ou_id source, int signal_mask,
                ou_observer_fn cb, void *ud);
void ou_unobserve(ou_context *ctx, ou_id source, ou_observer_fn cb);
void ou_notify(ou_context *ctx, ou_id source, int signal);

/* theme builder */
void     ou_theme_builder_init(ou_theme_builder *b);
void     ou_theme_builder_font(ou_theme_builder *b, ou_font font);
void     ou_theme_builder_padding(ou_theme_builder *b, int padding);
void     ou_theme_builder_spacing(ou_theme_builder *b, int spacing);
void     ou_theme_builder_color(ou_theme_builder *b, int tint, ou_color c);
void     ou_theme_builder_preset_dark(ou_theme_builder *b);
void     ou_theme_builder_preset_light(ou_theme_builder *b);
ou_theme ou_theme_builder_build(ou_theme_builder *b);

/* facade pattern */
int  ou_color_picker(ou_context *ctx, const char *label, ou_color *color);
int  ou_property_float(ou_context *ctx, const char *label, ou_real *value,
                       ou_real lo, ou_real hi);
int  ou_property_text(ou_context *ctx, const char *label, char *buf,
                      int bufsz);
int  ou_property_bool(ou_context *ctx, const char *label, int *state);
int  ou_confirm(ou_context *ctx, const char *title, const char *message,
                int *open);


/* convenience macros */
#define ou_button(ctx, label) \
    ou_button_ex(ctx, label, 0, OU_HINT_CENTER)

#define ou_textfield(ctx, buf, bufsz) \
    ou_textfield_ex(ctx, buf, bufsz, 0)

#define ou_slider(ctx, value, lo, hi) \
    ou_slider_ex(ctx, value, lo, hi, 0, OU_SLIDER_FMT, OU_HINT_CENTER)

#define ou_spinner(ctx, value, step) \
    ou_spinner_ex(ctx, value, step, OU_SLIDER_FMT, OU_HINT_CENTER)

#define ou_section(ctx, label) \
    ou_section_ex(ctx, label, 0)

#define ou_tree_begin(ctx, label) \
    ou_tree_begin_ex(ctx, label, 0)

#define ou_window_begin(ctx, title, rect) \
    ou_window_begin_ex(ctx, title, rect, 0)

#define ou_panel_begin(ctx, name) \
    ou_panel_begin_ex(ctx, name, 0)

#define ou_toggle(ctx, label, state) \
    ou_toggle_ex(ctx, label, state, OU_HINT_CENTER)

#define ou_switch(ctx, label, state) \
    ou_switch_ex(ctx, label, state, 0)

#define ou_progress(ctx, value) \
    ou_progress_ex(ctx, value, 0)

#define ou_meter(ctx, value, low, high) \
    ou_meter_ex(ctx, value, low, high, 0)

#define ou_avatar(ctx, initials) \
    ou_avatar_ex(ctx, initials, ou_color_make(90, 90, 200, 255), 0)

#define ou_field(ctx, label, buf, bufsz) \
    ou_field_ex(ctx, label, buf, bufsz, 0)

#define ou_collapsible_begin(ctx, label) \
    ou_collapsible_begin_ex(ctx, label, 0)

#define ou_scroll_area_begin(ctx, name, height) \
    ou_scroll_area_begin_ex(ctx, name, height, 0)

#endif
