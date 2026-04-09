#include "unity.h"
#include "test_support.h"

void test_radio_does_not_change_selection_without_press(void) {
    ou_context ctx;
    int selected = 1;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_radio(&ctx, "Option B", &selected, 2);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(1, selected);
}
