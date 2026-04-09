#include "unity.h"
#include "test_support.h"

void test_paragraph_and_label_emit_commands(void) {
    ou_context ctx;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    before = ctx.cmd_buf.idx;
    ou_paragraph(&ctx, "Alpha beta gamma delta epsilon.");
    ou_label(&ctx, "Final label");
    end_test_ui(&ctx);

    TEST_ASSERT_GREATER_THAN_INT(before, ctx.cmd_buf.idx);
}
