#include "unity.h"
#include "test_support.h"

void test_select_returns_zero_without_click(void) {
    ou_context ctx;
    const char *items[] = { "One", "Two", "Three" };
    int selected = 1;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_select(&ctx, items, 3, &selected);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(1, selected);
}
