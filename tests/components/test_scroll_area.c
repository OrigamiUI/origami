#include "unity.h"
#include "test_support.h"

void test_scroll_area_begin_end_balances_frame_stack(void) {
    ou_context ctx;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    before = ctx.frame_stack.idx;
    ou_scroll_area_begin_ex(&ctx, "Log", 120, 0);
    TEST_ASSERT_EQUAL_INT(before + 1, ctx.frame_stack.idx);
    ou_scroll_area_end(&ctx);
    TEST_ASSERT_EQUAL_INT(before, ctx.frame_stack.idx);
    end_test_ui(&ctx);
}
