#include "unity.h"
#include "test_support.h"

void test_checkbox_group_begin_end_restore_scope_and_indent(void) {
    ou_context ctx;
    int scope_before;
    int indent_before;

    init_ctx(&ctx);
    begin_test_ui(&ctx);

    scope_before = ctx.scope_stack.idx;
    indent_before = current_indent(&ctx);
    ou_checkbox_group_begin(&ctx, "Group A");

    TEST_ASSERT_EQUAL_INT(scope_before + 1, ctx.scope_stack.idx);
    TEST_ASSERT_EQUAL_INT(indent_before + ctx.theme->indent, current_indent(&ctx));

    ou_checkbox_group_end(&ctx);
    TEST_ASSERT_EQUAL_INT(scope_before, ctx.scope_stack.idx);
    TEST_ASSERT_EQUAL_INT(indent_before, current_indent(&ctx));

    end_test_ui(&ctx);
}

