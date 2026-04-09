#include "unity.h"
#include "test_support.h"

void test_toggle_group_without_press_keeps_selection(void) {
    ou_context ctx;
    const char *labels[] = { "A", "B", "C" };
    int selected = 0;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_toggle_group(&ctx, labels, 3, &selected);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(0, selected);
}
