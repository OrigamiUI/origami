#include "unity.h"
#include "test_support.h"

void test_checkbox_does_not_toggle_without_press(void) {
    ou_context ctx;
    int checked = 0;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_checkbox(&ctx, "Enabled", &checked);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(0, checked);
}
