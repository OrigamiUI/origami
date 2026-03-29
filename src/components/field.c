#include "../origami_internal.h"

int ou_field_ex(ou_context *ctx, const char *label, char *buf, int bufsz,
                int opt)
{
    int widths[2] = { 0, -1 };
    ou_row(ctx, 2, widths, 0);
    ou_label(ctx, label);
    return ou_textfield_ex(ctx, buf, bufsz, opt);
}

void ou_fieldset_begin(ou_context *ctx, const char *title) {
    int w = -1;
    ou_push_scope(ctx, title, (int) strlen(title));
    ou_row(ctx, 1, &w, 0);
    ou_inscribe(ctx, title, ou_allocate(ctx), OU_TINT_HEADING, 0);
    ou_separator(ctx);
    ou__current_grid(ctx)->indent += ctx->theme->indent;
}

void ou_fieldset_end(ou_context *ctx) {
    ou__current_grid(ctx)->indent -= ctx->theme->indent;
    ou_pop_scope(ctx);
}

int ou_property_text(ou_context *ctx, const char *label, char *buf,
                     int bufsz)
{
    return ou_field_ex(ctx, label, buf, bufsz, 0);
}

int ou_property_bool(ou_context *ctx, const char *label, int *state) {
    return ou_checkbox(ctx, label, state);
}

void ou_form_begin(ou_context *ctx, const char *name) {
    ou_push_scope(ctx, name, (int) strlen(name));
}

int ou_form_end(ou_context *ctx) {
    int res = 0;
    ou_separator(ctx);
    if (ou_button_ex(ctx, "Submit", 0, OU_HINT_CENTER)) {
        res |= OU_SIGNAL_COMMIT;
    }
    ou_pop_scope(ctx);
    return res;
}
