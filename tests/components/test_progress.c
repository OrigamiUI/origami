#include "unity.h"
#include "test_support.h"

void test_progress_and_meter_emit_commands(void) {
    ou_context ctx;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    before = ctx.cmd_buf.idx;
    ou_progress_ex(&ctx, 1.4f, 0);
    ou_meter_ex(&ctx, 12.0f, 0.0f, 10.0f, 0);
    end_test_ui(&ctx);

    TEST_ASSERT_GREATER_THAN_INT(before, ctx.cmd_buf.idx);
}
