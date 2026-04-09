#include "unity.h"
#include "test_support.h"

void test_switch_does_not_toggle_without_press(void) {
    ou_context ctx;
    int state = 0;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_switch_ex(&ctx, "Power", &state, 0);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(0, state);
}
