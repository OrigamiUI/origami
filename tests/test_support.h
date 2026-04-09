#ifndef TEST_SUPPORT_H
#define TEST_SUPPORT_H

#include "origami.h"

void init_ctx(ou_context *ctx);
void begin_test_ui(ou_context *ctx);
void end_test_ui(ou_context *ctx);
int current_indent(const ou_context *ctx);

#endif
