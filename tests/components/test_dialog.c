#include "unity.h"
#include "test_support.h"

void test_dialog_begin_respects_open_flag(void) {
    ou_context ctx;
    int open = 0;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    TEST_ASSERT_EQUAL_INT(0, ou_dialog_begin(&ctx, "Confirm", &open));

    open = 1;
    TEST_ASSERT_TRUE((ou_dialog_begin(&ctx, "Confirm", &open) & OU_SIGNAL_ACTIVE) != 0);
    ou_dialog_end(&ctx);
    end_test_ui(&ctx);
}
