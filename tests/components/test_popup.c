#include "unity.h"
#include "test_support.h"

void test_popup_begin_after_summon_is_active(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    ou_feed_motion(&ctx, 120, 88);
    ou_summon_popup(&ctx, "main-popup");
    sig = ou_popup_begin(&ctx, "main-popup");
    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
    ou_popup_end(&ctx);
    end_test_ui(&ctx);
}
