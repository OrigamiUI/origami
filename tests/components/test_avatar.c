#include "unity.h"
#include "test_support.h"

void test_avatar_ex_emits_draw_commands(void) {
    ou_context ctx;
    int before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    before = ctx.cmd_buf.idx;
    ou_avatar_ex(&ctx, "OU", ou_color_make(40, 80, 120, 255), 0);
    end_test_ui(&ctx);

    TEST_ASSERT_GREATER_THAN_INT(before, ctx.cmd_buf.idx);
}
