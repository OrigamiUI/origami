#include "unity.h"
#include "test_support.h"

void test_toggle_does_not_flip_without_press(void) {
    ou_context ctx;
    int state = 1;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_toggle_ex(&ctx, "Feature", &state, 0);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(1, state);
}
