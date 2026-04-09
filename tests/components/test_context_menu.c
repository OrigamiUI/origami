#include "unity.h"
#include "test_support.h"

void test_context_menu_begin_after_open_is_active(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    ctx.last_rect = ou_rect_make(16, 16, 80, 24);
    ou_open_context_menu(&ctx, "ctx-menu");
    sig = ou_context_menu_begin(&ctx, "ctx-menu");
    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
    ou_context_menu_end(&ctx);
    end_test_ui(&ctx);
}
