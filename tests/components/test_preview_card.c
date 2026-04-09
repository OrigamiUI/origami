#include "unity.h"
#include "test_support.h"

void test_preview_card_begin_when_hovered_is_active(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    ctx.last_rect = ou_rect_make(40, 40, 90, 20);
    ou_feed_motion(&ctx, 45, 45);
    ctx.hover_root = ou_active_frame(&ctx);
    sig = ou_preview_card_begin(&ctx, "preview");
    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
    ou_preview_card_end(&ctx);
    end_test_ui(&ctx);
}
