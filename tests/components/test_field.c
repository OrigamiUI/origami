#include "unity.h"
#include "test_support.h"

void test_fieldset_and_form_balance_scope(void) {
    ou_context ctx;
    int scope_before;
    int indent_before;
    char buf[32] = "";

    init_ctx(&ctx);
    begin_test_ui(&ctx);

    scope_before = ctx.scope_stack.idx;
    indent_before = current_indent(&ctx);

    TEST_ASSERT_EQUAL_INT(0, ou_field_ex(&ctx, "Name", buf, (int) sizeof(buf), 0));

    ou_fieldset_begin(&ctx, "Account");
    TEST_ASSERT_EQUAL_INT(scope_before + 1, ctx.scope_stack.idx);
    TEST_ASSERT_EQUAL_INT(indent_before + ctx.theme->indent, current_indent(&ctx));
    ou_fieldset_end(&ctx);
    TEST_ASSERT_EQUAL_INT(scope_before, ctx.scope_stack.idx);
    TEST_ASSERT_EQUAL_INT(indent_before, current_indent(&ctx));

    ou_form_begin(&ctx, "profile-form");
    TEST_ASSERT_EQUAL_INT(scope_before + 1, ctx.scope_stack.idx);
    TEST_ASSERT_EQUAL_INT(0, ou_form_end(&ctx));
    TEST_ASSERT_EQUAL_INT(scope_before, ctx.scope_stack.idx);

    end_test_ui(&ctx);
}
