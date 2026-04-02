#include <SDL.h>
#include <string.h>
#include "renderer.h"
#include "atlas.inl"

static int win_w  = 1600;
static int win_h  = 1200;

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *atlas_tex;

/* pre-rendered RGBA atlas (built once from the alpha-only source) */
static unsigned char atlas_rgba[ATLAS_WIDTH * ATLAS_HEIGHT * 4];


void r_init(void) {
    window = SDL_CreateWindow(
        "Origami UI Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        win_w, win_h, 0);
    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    /* convert alpha-only atlas to RGBA white with alpha */
    for (int i = 0; i < ATLAS_WIDTH * ATLAS_HEIGHT; i++) {
        atlas_rgba[i * 4 + 0] = 255;
        atlas_rgba[i * 4 + 1] = 255;
        atlas_rgba[i * 4 + 2] = 255;
        atlas_rgba[i * 4 + 3] = atlas_texture[i];
    }

    atlas_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STATIC, ATLAS_WIDTH, ATLAS_HEIGHT);
    SDL_UpdateTexture(atlas_tex, NULL, atlas_rgba, ATLAS_WIDTH * 4);
    SDL_SetTextureBlendMode(atlas_tex, SDL_BLENDMODE_BLEND);
}


static void draw_atlas_region(ou_rect dst, ou_rect src, ou_color color) {
    SDL_Rect s = { src.x, src.y, src.w, src.h };
    SDL_Rect d = { dst.x, dst.y, dst.w, dst.h };
    SDL_SetTextureColorMod(atlas_tex, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(atlas_tex, color.a);
    SDL_RenderCopy(renderer, atlas_tex, &s, &d);
}


void r_draw_rect(ou_rect rect, ou_color color) {
    draw_atlas_region(rect, atlas[ATLAS_WHITE], color);
}


void r_draw_text(const char *text, ou_vec2 pos, ou_color color) {
    ou_rect dst = { pos.x, pos.y, 0, 0 };
    for (const char *p = text; *p; p++) {
        if ((*p & 0xc0) == 0x80) { continue; }
        int chr = ou_min((unsigned char) *p, 127);
        ou_rect src = atlas[ATLAS_FONT + chr];
        dst.w = src.w;
        dst.h = src.h;
        draw_atlas_region(dst, src, color);
        dst.x += dst.w;
    }
}


void r_draw_icon(int id, ou_rect rect, ou_color color) {
    ou_rect src = atlas[id];
    int x = rect.x + (rect.w - src.w) / 2;
    int y = rect.y + (rect.h - src.h) / 2;
    draw_atlas_region(ou_rect_make(x, y, src.w, src.h), src, color);
}


int r_get_text_width(const char *text, int len) {
    int res = 0;
    for (const char *p = text; *p && len--; p++) {
        if ((*p & 0xc0) == 0x80) { continue; }
        int chr = ou_min((unsigned char) *p, 127);
        res += atlas[ATLAS_FONT + chr].w;
    }
    return res;
}


int r_get_text_height(void) {
    return 18;
}


void r_set_clip_rect(ou_rect rect) {
    SDL_Rect r = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderSetClipRect(renderer, &r);
}


void r_clear(ou_color clr) {
    SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderClear(renderer);
}


void r_present(void) {
    SDL_RenderPresent(renderer);
}
