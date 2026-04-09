#include "unity.h"
#include "test_support.h"

void test_toolbar_begin_end_balances_grid_depth(void) {
    ou_context ctx;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    before = ctx.grid_stack.idx;
    ou_toolbar_begin(&ctx);
    TEST_ASSERT_EQUAL_INT(before + 1, ctx.grid_stack.idx);
    ou_toolbar_end(&ctx);
    TEST_ASSERT_EQUAL_INT(before, ctx.grid_stack.idx);
    end_test_ui(&ctx);
}
