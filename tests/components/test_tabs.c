#include "unity.h"
#include "test_support.h"

void test_tabs_without_press_keep_selection(void) {
    ou_context ctx;
    const char *labels[] = { "General", "Advanced", "About" };
    int selected = 2;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_tabs(&ctx, labels, 3, &selected);
    end_test_ui(&ctx);

    TEST_ASSERT_EQUAL_INT(0, sig);
    TEST_ASSERT_EQUAL_INT(2, selected);
}
