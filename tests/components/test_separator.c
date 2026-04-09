#include "unity.h"
#include "test_support.h"

void test_separator_emits_line_command(void) {
    ou_context ctx;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    before = ctx.cmd_buf.idx;
    ou_separator(&ctx);
    end_test_ui(&ctx);

    TEST_ASSERT_GREATER_THAN_INT(before, ctx.cmd_buf.idx);
}
