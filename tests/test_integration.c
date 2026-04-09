#include "unity.h"
#include "test_support.h"

#include <string.h>

static int count_non_skip_commands(ou_context *ctx) {
    ou_cmd *cmd = NULL;
    int count = 0;
    while (ou_walk(ctx, &cmd)) {
        TEST_ASSERT_NOT_EQUAL_INT(OU_CMD_SKIP, cmd->type);
        count++;
    }
    return count;
}

static ou_color first_fill_color(ou_context *ctx) {
    ou_cmd *cmd = NULL;
    while (ou_walk(ctx, &cmd)) {
        if (cmd->type == OU_CMD_FILL) {
            return cmd->fill.color;
        }
    }
    return ou_color_make(0, 0, 0, 0);
}

void test_command_pipeline_walk_returns_draw_commands(void) {
    ou_context ctx;
    int command_count;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    (void) ou_button_ex(&ctx, "Pipeline", 0, 0);
    end_test_ui(&ctx);

    command_count = count_non_skip_commands(&ctx);
    TEST_ASSERT_GREATER_THAN_INT(0, command_count);
}

void test_textfield_enter_returns_commit_and_clears_focus(void) {
    ou_context ctx;
    char buf[32] = "abc";
    ou_id id = 0x5678u;
    ou_rect r;
    int sig;

    init_ctx(&ctx);
    begin_test_ui(&ctx);
    r = ou_allocate(&ctx);
    ctx.focus = id;
    ou_feed_keydown(&ctx, OU_KEY_ENTER);

    sig = ou_textfield_raw(&ctx, buf, (int) sizeof(buf), id, r, 0);
    end_test_ui(&ctx);

    TEST_ASSERT_TRUE((sig & OU_SIGNAL_COMMIT) != 0);
    TEST_ASSERT_EQUAL_UINT(0u, ctx.focus);
}

void test_theme_change_affects_rendered_fill_color_next_frame(void) {
    ou_context ctx;
    ou_color before;
    ou_color after;

    init_ctx(&ctx);

    begin_test_ui(&ctx);
    (void) ou_button_ex(&ctx, "Theme", 0, 0);
    end_test_ui(&ctx);
    before = first_fill_color(&ctx);

    ctx.theme->colors[OU_TINT_CANVAS] = ou_color_make(12, 34, 56, 255);
    begin_test_ui(&ctx);
    (void) ou_button_ex(&ctx, "Theme", 0, 0);
    end_test_ui(&ctx);
    after = first_fill_color(&ctx);

    TEST_ASSERT_NOT_EQUAL_INT(before.r, after.r);
    TEST_ASSERT_NOT_EQUAL_INT(before.g, after.g);
    TEST_ASSERT_NOT_EQUAL_INT(before.b, after.b);
    TEST_ASSERT_EQUAL_INT(12, after.r);
    TEST_ASSERT_EQUAL_INT(34, after.g);
    TEST_ASSERT_EQUAL_INT(56, after.b);
}
