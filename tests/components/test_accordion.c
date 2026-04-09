#include "unity.h"
#include "test_support.h"

void test_accordion_item_reports_active_when_selected(void) {
    ou_context ctx;
    int active = 2;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_accordion_item(&ctx, "Section", &active, 2);
    end_test_ui(&ctx);

    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
    TEST_ASSERT_EQUAL_INT(2, active);
}
