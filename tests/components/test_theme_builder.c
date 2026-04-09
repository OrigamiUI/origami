#include "unity.h"
#include "test_support.h"

void test_theme_builder_applies_mutations(void) {
    ou_theme_builder b;
    ou_theme t;

    ou_theme_builder_init(&b);
    ou_theme_builder_padding(&b, 7);
    ou_theme_builder_spacing(&b, 9);
    ou_theme_builder_color(&b, OU_TINT_ACCENT, ou_color_make(1, 2, 3, 255));
    t = ou_theme_builder_build(&b);
    TEST_ASSERT_EQUAL_INT(7, t.padding);
    TEST_ASSERT_EQUAL_INT(9, t.spacing);
    TEST_ASSERT_EQUAL_INT(1, t.colors[OU_TINT_ACCENT].r);
    TEST_ASSERT_EQUAL_INT(2, t.colors[OU_TINT_ACCENT].g);
    TEST_ASSERT_EQUAL_INT(3, t.colors[OU_TINT_ACCENT].b);

    ou_theme_builder_preset_light(&b);
    t = ou_theme_builder_build(&b);
    TEST_ASSERT_EQUAL_INT(245, t.colors[OU_TINT_CANVAS].r);
}
