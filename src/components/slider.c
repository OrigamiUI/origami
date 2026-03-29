#include "../origami_internal.h"

int ou_slider_ex(ou_context *ctx, ou_real *value, ou_real low, ou_real high,
                 ou_real step, const char *fmt, int opt)
{
    char buf[OU_FMT_LIMIT + 1];
    ou_rect thumb;
    int x, w, res = 0;
    ou_real last = *value, v = last;
    ou_id id = ou_hash(ctx, &value, sizeof(value));
    ou_rect base = ou_allocate(ctx);

    if (ou__number_textfield(ctx, &v, base, id)) { return res; }

    ou_sense(ctx, id, base, opt);

    if (ctx->focus == id &&
        (ctx->mouse_down | ctx->mouse_pressed) == OU_MOUSE_PRIMARY)
    {
        v = low + (ctx->mouse_pos.x - base.x) * (high - low) / base.w;
        if (step) {
            v = (ou_real)(((long long)((v + step / 2) / step)) * step);
        }
    }
    *value = v = ou_clamp(v, low, high);
    if (last != v) {
        res |= OU_SIGNAL_ALTERED;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }

    ou_decorate(ctx, id, base, OU_TINT_FIELD, opt);
    w = ctx->theme->thumb_size;
    x = (int) ((v - low) * (base.w - w) / (high - low));
    thumb = ou_rect_make(base.x + x, base.y, w, base.h);
    ou_decorate(ctx, id, thumb, OU_TINT_CONTROL, opt);
    sprintf(buf, fmt, v);
    ou_inscribe(ctx, buf, base, OU_TINT_LABEL, opt);

    return res;
}

int ou_spinner_ex(ou_context *ctx, ou_real *value, ou_real step,
                  const char *fmt, int opt)
{
    char buf[OU_FMT_LIMIT + 1];
    int res = 0;
    ou_id id = ou_hash(ctx, &value, sizeof(value));
    ou_rect base = ou_allocate(ctx);
    ou_real last = *value;

    if (ou__number_textfield(ctx, value, base, id)) { return res; }

    ou_sense(ctx, id, base, opt);

    if (ctx->focus == id && ctx->mouse_down == OU_MOUSE_PRIMARY) {
        *value += ctx->mouse_delta.x * step;
    }
    if (*value != last) {
        res |= OU_SIGNAL_ALTERED;
        ou_notify(ctx, id, OU_SIGNAL_ALTERED);
    }

    ou_decorate(ctx, id, base, OU_TINT_FIELD, opt);
    sprintf(buf, fmt, *value);
    ou_inscribe(ctx, buf, base, OU_TINT_LABEL, opt);

    return res;
}

int ou_color_picker(ou_context *ctx, const char *label, ou_color *color) {
    int res = 0;
    float r_val, g_val, b_val;
    ou_push_scope(ctx, label, (int) strlen(label));

    ou_row(ctx, 1, (int[]) { -1 }, 0);
    ou_label(ctx, label);

    r_val = (float) color->r;
    g_val = (float) color->g;
    b_val = (float) color->b;

    ou_row(ctx, 2, (int[]) { 40, -1 }, 0);
    ou_label(ctx, "R:");
    if (ou_slider_ex(ctx, &r_val, 0, 255, 1, "%.0f", OU_HINT_CENTER)) {
        res |= OU_SIGNAL_ALTERED;
    }
    ou_label(ctx, "G:");
    if (ou_slider_ex(ctx, &g_val, 0, 255, 1, "%.0f", OU_HINT_CENTER)) {
        res |= OU_SIGNAL_ALTERED;
    }
    ou_label(ctx, "B:");
    if (ou_slider_ex(ctx, &b_val, 0, 255, 1, "%.0f", OU_HINT_CENTER)) {
        res |= OU_SIGNAL_ALTERED;
    }

    color->r = (unsigned char) r_val;
    color->g = (unsigned char) g_val;
    color->b = (unsigned char) b_val;

    {
        ou_rect swatch = ou_allocate(ctx);
        ou_paint_rect(ctx, swatch, *color);
    }

    ou_pop_scope(ctx);
    return res;
}

int ou_property_float(ou_context *ctx, const char *label, ou_real *value,
                      ou_real lo, ou_real hi)
{
    int widths[2] = { 0, -1 };
    ou_row(ctx, 2, widths, 0);
    ou_label(ctx, label);
    return ou_slider_ex(ctx, value, lo, hi, 0, OU_SLIDER_FMT,
                        OU_HINT_CENTER);
}
