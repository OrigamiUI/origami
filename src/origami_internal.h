/*
** Origami UI - Internal Header
** Shared macros and declarations for component files.
*/

#ifndef ORIGAMI_INTERNAL_H
#define ORIGAMI_INTERNAL_H

#include "origami.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define unused(x) ((void) (x))

#define verify(x) do {                                                 \
        if (!(x)) {                                                    \
            fprintf(stderr, "Fatal error: %s:%d: assertion '%s' failed\n", \
                __FILE__, __LINE__, #x);                               \
            abort();                                                   \
        }                                                              \
    } while (0)

#define push(stk, val) do {                                                     \
        verify((stk).idx < (int) (sizeof((stk).items) / sizeof(*(stk).items))); \
        (stk).items[(stk).idx] = (val);                                         \
        (stk).idx++;                                                            \
    } while (0)

#define pop(stk) do {        \
        verify((stk).idx > 0); \
        (stk).idx--;           \
    } while (0)


/* internal globals */
extern ou_rect  ou__infinite_rect;
extern ou_theme ou__base_theme;

/* internal rect helpers */
ou_rect ou__grow_rect(ou_rect r, int n);
ou_rect ou__overlap_rects(ou_rect a, ou_rect b);
int     ou__point_in_rect(ou_rect r, ou_vec2 p);

/* internal state access */
int      ou__in_hover_root(ou_context *ctx);
ou_grid *ou__current_grid(ou_context *ctx);

/* frame management internals */
ou_frame *ou__resolve_frame(ou_context *ctx, ou_id id, int opt);
void      ou__begin_root(ou_context *ctx, ou_frame *frm);
void      ou__end_root(ou_context *ctx);
void      ou__push_frame_body(ou_context *ctx, ou_frame *frm, ou_rect body,
                              int opt);
void      ou__push_grid(ou_context *ctx, ou_rect body, ou_vec2 scroll);
void      ou__pop_frame_state(ou_context *ctx);
ou_cmd   *ou__emit_skip(ou_context *ctx, ou_cmd *dst);
void      ou__handle_scrollbars(ou_context *ctx, ou_frame *frm,
                                ou_rect *body);

/* shared widget helpers */
int ou__collapsible(ou_context *ctx, const char *label, int is_tree, int opt);
int ou__number_textfield(ou_context *ctx, ou_real *value, ou_rect r, ou_id id);

#endif
