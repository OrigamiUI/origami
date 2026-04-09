#include "unity.h"
#include "test_support.h"

void test_textfield_raw_appends_input_when_focused(void) {
    ou_context ctx;
    char buf[32] = "a";
    ou_id id = 0x1234u;
    ou_rect r;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);

    r = ou_allocate(&ctx);
    ctx.focus = id;
    ou_feed_text(&ctx, "bc");

    sig = ou_textfield_raw(&ctx, buf, (int) sizeof(buf), id, r, 0);
    end_test_ui(&ctx);

    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ALTERED) != 0);
    TEST_ASSERT_EQUAL_STRING("abc", buf);
}
