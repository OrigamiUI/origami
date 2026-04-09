#include "unity.h"
#include "test_support.h"

void test_section_ex_unfolded_is_active(void) {
    ou_context ctx;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_section_ex(&ctx, "General", OU_HINT_UNFOLDED);
    end_test_ui(&ctx);

    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ACTIVE) != 0);
}
