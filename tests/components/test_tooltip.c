#include "unity.h"
#include "test_support.h"

void test_tooltip_emits_overlay_when_anchor_is_hovered(void) {
    ou_context ctx;
    ou_rect anchor;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);

    ou_label(&ctx, "Hover me");
    anchor = ctx.last_rect;
    ou_feed_motion(&ctx, anchor.x + 1, anchor.y + 1);
    ctx.hover_root = ou_active_frame(&ctx);

    before = ctx.cmd_buf.idx;
    ou_tooltip(&ctx, "Tooltip body");
    end_test_ui(&ctx);

    TEST_ASSERT_GREATER_THAN_INT(before, ctx.cmd_buf.idx);
}
