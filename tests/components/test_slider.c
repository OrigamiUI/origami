#include "unity.h"
#include "test_support.h"

void test_slider_clamps_value_to_range(void) {
    ou_context ctx;
    ou_real value = 99.0f;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    sig = ou_slider_ex(&ctx, &value, 0.0f, 10.0f, 0.0f, OU_SLIDER_FMT, 0);
    end_test_ui(&ctx);

    TEST_ASSERT_TRUE((sig & OU_SIGNAL_ALTERED) != 0);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 10.0f, value);
}
