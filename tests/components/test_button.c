#include "unity.h"
#include "test_support.h"

void test_button_ex_returns_zero_without_press(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_button_ex(&ctx, "Save", 0, OU_HINT_CENTER);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
}

