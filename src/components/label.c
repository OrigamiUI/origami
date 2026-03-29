#include "../origami.h"

void ou_paragraph(ou_context *ctx, const char *text) {
    const char *start, *end, *p = text;
    int width = -1;
    ou_font font = ctx->theme->font;
    ou_color color = ctx->theme->colors[OU_TINT_LABEL];
    ou_column_begin(ctx);
    ou_row(ctx, 1, &width, ctx->text_height(font));
    do {
        ou_rect r = ou_allocate(ctx);
        int w = 0;
        start = end = p;
        do {
            const char *word = p;
            while (*p && *p != ' ' && *p != '\n') { p++; }
            w += ctx->text_width(font, word, (int) (p - word));
            if (w > r.w && end != start) { break; }
            w += ctx->text_width(font, p, 1);
            end = p++;
        } while (*end && *end != '\n');
        ou_paint_text(ctx, font, start, (int) (end - start),
                      ou_vec2_make(r.x, r.y), color);
        p = end + 1;
    } while (*end);
    ou_column_end(ctx);
}

void ou_label(ou_context *ctx, const char *text) {
    ou_inscribe(ctx, text, ou_allocate(ctx), OU_TINT_LABEL, 0);
}
