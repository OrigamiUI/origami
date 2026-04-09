#include "unity.h"
#include "test_support.h"

void test_popover_begin_after_summon_is_active(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    ctx.last_rect = ou_rect_make(40, 40, 100, 22);
    ou_summon_popover(&ctx, "color-popover");
    sig = ou_popover_begin(&ctx, "color-popover");
    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
    ou_popover_end(&ctx);
    end_test_ui(&ctx);
}
