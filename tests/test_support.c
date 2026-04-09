#include "unity.h"
#include "test_support.h"

#include <string.h>

static int stub_text_width(ou_font font, const char *str, int len) {
    (void) font;
    if (!str) { return 0; }
    if (len < 0) { len = (int) strlen(str); }
    return len * 8;
}

static int stub_text_height(ou_font font) {
    (void) font;
    return 14;
}

int current_indent(const ou_context *ctx) {
    return ctx->grid_stack.items[ctx->grid_stack.idx - 1].indent;
}

void init_ctx(ou_context *ctx) {
    ou_setup(ctx);
    ctx->text_width = stub_text_width;
    ctx->text_height = stub_text_height;
    ctx->theme->font = (ou_font) 1;
}

void begin_test_ui(ou_context *ctx) {
    int widths[1] = { -1 };
    ou_frame_begin(ctx);
    TEST_ASSERT_TRUE((ou_window_begin_ex(
        ctx,
        "Root",
        ou_rect_make(0, 0, 420, 320),
        OU_HINT_PERMANENT | OU_HINT_FIXED) & OU_SIGNAL_ACTIVE) != 0);
    ctx->hover_root = ou_active_frame(ctx);
    ou_row(ctx, 1, widths, 24);
}

void end_test_ui(ou_context *ctx) {
    ou_window_end(ctx);
    ou_frame_end(ctx);
}

void setUp(void) {}
void tearDown(void) {}
