#include "unity.h"
#include "test_support.h"

void test_window_begin_end_works_standalone(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    ou_frame_begin(&ctx);
    sig = ou_window_begin_ex(
        &ctx,
        "Standalone",
        ou_rect_make(0, 0, 240, 180),
        OU_HINT_PERMANENT | OU_HINT_FIXED);

    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
    ou_window_end(&ctx);
    ou_frame_end(&ctx);

    TEST_ASSERT_EQUAL_INT(0, ctx.frame_stack.idx);
    TEST_ASSERT_EQUAL_INT(0, ctx.grid_stack.idx);
    TEST_ASSERT_EQUAL_INT(0, ctx.scissor_stack.idx);
}
