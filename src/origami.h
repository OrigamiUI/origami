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

#endif

#endif
