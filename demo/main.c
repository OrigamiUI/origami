#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "renderer.h"
#include "origami.h"


static  char logbuf[64000];
static   int logbuf_dirty = 0;
static float bg[3] = { 90, 95, 100 };


static void append_log(const char *text) {
    if (logbuf[0]) { strcat(logbuf, "\n"); }
    strcat(logbuf, text);
    logbuf_dirty = 1;
}

static void observer_log(ou_context *ctx, ou_id source, int signal,
                         void *userdata)
{
    (void) ctx; (void) source; (void) userdata;
    if (signal & OU_SIGNAL_COMMIT)  { append_log("Observer: commit"); }
    if (signal & OU_SIGNAL_ALTERED) { append_log("Observer: altered"); }
}


static void showcase_window(ou_context *ctx) {
    if (ou_window_begin(ctx, "Demo Window", ou_rect_make(40, 40, 300, 450))) {
        ou_frame *win = ou_active_frame(ctx);
        win->rect.w = ou_max(win->rect.w, 240);
        win->rect.h = ou_max(win->rect.h, 300);

        /* window info */
        if (ou_section(ctx, "Window Info")) {
            ou_frame *cur = ou_active_frame(ctx);
            char buf[64];
            ou_row(ctx, 2, (int[]) { 54, -1 }, 0);
            ou_label(ctx, "Position:");
            sprintf(buf, "%d, %d", cur->rect.x, cur->rect.y);
            ou_label(ctx, buf);
            ou_label(ctx, "Size:");
            sprintf(buf, "%d, %d", cur->rect.w, cur->rect.h);
            ou_label(ctx, buf);
        }

        /* buttons & toggle */
        if (ou_section_ex(ctx, "Buttons & Toggle", OU_HINT_UNFOLDED)) {
            ou_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
            ou_label(ctx, "Buttons:");
            if (ou_button(ctx, "Button 1")) { append_log("Pressed button 1"); }
            if (ou_button(ctx, "Button 2")) { append_log("Pressed button 2"); }
            ou_label(ctx, "Toggle:");
            static int tog1 = 0, tog2 = 1;
            if (ou_toggle(ctx, "Bold", &tog1)) { append_log("Toggled bold"); }
            if (ou_toggle(ctx, "Italic", &tog2)) { append_log("Toggled italic"); }
            ou_label(ctx, "Popup:");
            if (ou_button(ctx, "Open")) { ou_summon_popup(ctx, "Test Popup"); }
            ou_label(ctx, "");
            if (ou_popup_begin(ctx, "Test Popup")) {
                ou_button(ctx, "Hello");
                ou_button(ctx, "World");
                ou_popup_end(ctx);
            }
        }

        /* checkbox & switch */
        if (ou_section_ex(ctx, "Checkbox & Switch", OU_HINT_UNFOLDED)) {
            static int checks[3] = { 1, 0, 1 };
            static int sw1 = 1, sw2 = 0;
            static int watched = 0;
            static int obs_registered = 0;
            ou_checkbox(ctx, "Checkbox 1", &checks[0]);
            ou_checkbox(ctx, "Checkbox 2", &checks[1]);
            ou_checkbox(ctx, "Checkbox 3", &checks[2]);
            /* observed checkbox — fires callback on toggle */
            if (!obs_registered) {
                ou_id wid = ou_hash(ctx, &watched, sizeof(watched));
                ou_observe(ctx, wid, OU_SIGNAL_ALTERED, observer_log, NULL);
                obs_registered = 1;
            }
            ou_checkbox(ctx, "Watched (observer)", &watched);
            ou_separator(ctx);
            ou_switch(ctx, "Notifications", &sw1);
            ou_switch(ctx, "Dark mode", &sw2);
        }

        /* input */
        if (ou_section_ex(ctx, "Text Input", OU_HINT_UNFOLDED)) {
            static char name[128] = "Origami";
            static char email[128] = "";
            ou_row(ctx, 2, (int[]) { 46, -1 }, 0);
            ou_label(ctx, "Name:");
            ou_textfield(ctx, name, sizeof(name));
            ou_label(ctx, "Email:");
            ou_textfield(ctx, email, sizeof(email));
        }

        /* sliders */
        if (ou_section_ex(ctx, "Sliders", OU_HINT_UNFOLDED)) {
            static float val1 = 50, val2 = 0.5f;
            ou_row(ctx, 2, (int[]) { 46, -1 }, 0);
            ou_label(ctx, "Value:");
            ou_slider(ctx, &val1, 0, 100);
            ou_label(ctx, "Ratio:");
            ou_slider_ex(ctx, &val2, 0, 1, 0.01f, "%.2f", OU_HINT_CENTER);
        }

        /* progress & meter */
        if (ou_section_ex(ctx, "Progress & Meter", OU_HINT_UNFOLDED)) {
            static float prog = 0;
            prog += 0.002f;
            if (prog > 1.0f) { prog = 0; }
            ou_row(ctx, 2, (int[]) { 60, -1 }, 0);
            ou_label(ctx, "Progress:");
            ou_progress(ctx, prog);
            ou_label(ctx, "Meter:");
            ou_meter(ctx, 72.5f, 0, 100);
        }

        /* avatar */
        if (ou_section_ex(ctx, "Avatars", OU_HINT_UNFOLDED)) {
            ou_row(ctx, 4, (int[]) { 28, 28, 28, -1 }, 28);
            ou_avatar(ctx, "AB");
            ou_avatar_ex(ctx, "CD", ou_color_make(180, 80, 80, 255), 0);
            ou_avatar_ex(ctx, "EF", ou_color_make(80, 160, 80, 255), 0);
            ou_label(ctx, "User avatars");
        }

        /* tree and text */
        if (ou_section(ctx, "Tree and Text")) {
            ou_row(ctx, 2, (int[]) { 140, -1 }, 0);
            ou_column_begin(ctx);
            if (ou_tree_begin(ctx, "Test 1")) {
                if (ou_tree_begin(ctx, "Test 1a")) {
                    ou_label(ctx, "Hello");
                    ou_label(ctx, "world");
                    ou_tree_end(ctx);
                }
                if (ou_tree_begin(ctx, "Test 1b")) {
                    if (ou_button(ctx, "Button 1")) {
                        append_log("Pressed button 1");
                    }
                    if (ou_button(ctx, "Button 2")) {
                        append_log("Pressed button 2");
                    }
                    ou_tree_end(ctx);
                }
                ou_tree_end(ctx);
            }
            if (ou_tree_begin(ctx, "Test 2")) {
                ou_row(ctx, 2, (int[]) { 54, 54 }, 0);
                if (ou_button(ctx, "Button 3")) { append_log("Pressed button 3"); }
                if (ou_button(ctx, "Button 4")) { append_log("Pressed button 4"); }
                ou_tree_end(ctx);
            }
            ou_column_end(ctx);

            ou_column_begin(ctx);
            ou_row(ctx, 1, (int[]) { -1 }, 0);
            ou_paragraph(ctx,
                "Lorem ipsum dolor sit amet, consectetur adipiscing "
                "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus "
                "faucibus ipsum, eu varius magna felis a nulla.");
            ou_column_end(ctx);
        }

        /* background color sliders */
        if (ou_section(ctx, "Background Color")) {
            ou_row(ctx, 2, (int[]) { -78, -1 }, 74);
            ou_column_begin(ctx);
            ou_row(ctx, 2, (int[]) { 46, -1 }, 0);
            ou_label(ctx, "Red:");   ou_slider(ctx, &bg[0], 0, 255);
            ou_label(ctx, "Green:"); ou_slider(ctx, &bg[1], 0, 255);
            ou_label(ctx, "Blue:");  ou_slider(ctx, &bg[2], 0, 255);
            ou_column_end(ctx);
            ou_rect r = ou_allocate(ctx);
            ou_paint_rect(ctx, r,
                          ou_color_make((int) bg[0], (int) bg[1], (int) bg[2], 255));
            char buf[32];
            sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
            ou_inscribe(ctx, buf, r, OU_TINT_LABEL, OU_HINT_CENTER);
        }

        ou_window_end(ctx);
    }
}


static void journal_window(ou_context *ctx) {
    if (ou_window_begin(ctx, "Log Window",
                        ou_rect_make(350, 40, 300, 200)))
    {
        /* output text panel */
        ou_row(ctx, 1, (int[]) { -1 }, -25);
        ou_panel_begin(ctx, "Log Output");
        ou_frame *panel = ou_active_frame(ctx);
        ou_row(ctx, 1, (int[]) { -1 }, -1);
        ou_paragraph(ctx, logbuf);
        ou_panel_end(ctx);
        if (logbuf_dirty) {
            panel->scroll.y = panel->content_size.y;
            logbuf_dirty = 0;
        }

        /* input textfield + submit button */
        static char buf[128];
        int submitted = 0;
        ou_row(ctx, 2, (int[]) { -70, -1 }, 0);
        if (ou_textfield(ctx, buf, sizeof(buf)) & OU_SIGNAL_COMMIT) {
            ou_grant_focus(ctx, ctx->last_id);
            submitted = 1;
        }
        if (ou_button(ctx, "Submit")) { submitted = 1; }
        if (submitted) {
            append_log(buf);
            buf[0] = '\0';
        }

        ou_window_end(ctx);
    }
}


static int byte_slider(ou_context *ctx, unsigned char *value, int low,
                       int high)
{
    static float tmp;
    ou_push_scope(ctx, &value, sizeof(value));
    tmp = *value;
    int res = ou_slider_ex(ctx, &tmp, (ou_real) low, (ou_real) high, 0,
                           "%.0f", OU_HINT_CENTER);
    *value = (unsigned char) tmp;
    ou_pop_scope(ctx);
    return res;
}


static void palette_window(ou_context *ctx) {
    static struct { const char *label; int idx; } colors[] = {
        { "label:",        OU_TINT_LABEL        },
        { "edge:",         OU_TINT_EDGE         },
        { "canvas:",       OU_TINT_CANVAS       },
        { "header:",       OU_TINT_HEADER       },
        { "heading:",      OU_TINT_HEADING      },
        { "surface:",      OU_TINT_SURFACE      },
        { "control:",      OU_TINT_CONTROL      },
        { "ctrl warm:",    OU_TINT_CONTROL_WARM },
        { "ctrl hot:",     OU_TINT_CONTROL_HOT  },
        { "field:",        OU_TINT_FIELD        },
        { "field warm:",   OU_TINT_FIELD_WARM   },
        { "field hot:",    OU_TINT_FIELD_HOT    },
        { "track:",        OU_TINT_TRACK        },
        { "handle:",       OU_TINT_HANDLE       },
        { "accent:",       OU_TINT_ACCENT       },
        { NULL, 0 }
    };

    if (ou_window_begin(ctx, "Style Editor",
                        ou_rect_make(350, 250, 300, 240)))
    {
        /* theme presets */
        ou_row(ctx, 2, (int[]) { 0, 0 }, 0);
        if (ou_button(ctx, "Dark Theme")) {
            ou_theme_builder b;
            ou_theme_builder_init(&b);
            ou_theme_builder_preset_dark(&b);
            ctx->_theme = ou_theme_builder_build(&b);
        }
        if (ou_button(ctx, "Light Theme")) {
            ou_theme_builder b;
            ou_theme_builder_init(&b);
            ou_theme_builder_preset_light(&b);
            ctx->_theme = ou_theme_builder_build(&b);
        }
        ou_separator(ctx);

        /* per-color sliders */
        int sw = (int) (ou_active_frame(ctx)->body.w * 0.14);
        ou_row(ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
        for (int i = 0; colors[i].label; i++) {
            ou_label(ctx, colors[i].label);
            byte_slider(ctx, &ctx->theme->colors[colors[i].idx].r, 0, 255);
            byte_slider(ctx, &ctx->theme->colors[colors[i].idx].g, 0, 255);
            byte_slider(ctx, &ctx->theme->colors[colors[i].idx].b, 0, 255);
            byte_slider(ctx, &ctx->theme->colors[colors[i].idx].a, 0, 255);
            ou_paint_rect(ctx, ou_allocate(ctx),
                          ctx->theme->colors[colors[i].idx]);
        }

        /* color picker */
        ou_separator(ctx);
        static ou_color pick = { 100, 150, 200, 255 };
        ou_color_picker(ctx, "Custom color:", &pick);

        ou_window_end(ctx);
    }
}


static void compound_window(ou_context *ctx) {
    if (ou_window_begin(ctx, "Compound Widgets",
                        ou_rect_make(660, 40, 320, 500)))
    {
        ou_frame *win = ou_active_frame(ctx);
        win->rect.w = ou_max(win->rect.w, 280);

        /* radio buttons */
        if (ou_section_ex(ctx, "Radio Buttons", OU_HINT_UNFOLDED)) {
            static int choice = 0;
            ou_radio(ctx, "Option A", &choice, 0);
            ou_radio(ctx, "Option B", &choice, 1);
            ou_radio(ctx, "Option C", &choice, 2);
        }

        /* toggle group */
        if (ou_section_ex(ctx, "Toggle Group", OU_HINT_UNFOLDED)) {
            static int align = 0;
            const char *aligns[] = { "Left", "Center", "Right" };
            ou_toggle_group(ctx, aligns, 3, &align);
        }

        /* checkbox group */
        if (ou_section_ex(ctx, "Checkbox Group", OU_HINT_UNFOLDED)) {
            static int opts[3] = { 1, 0, 1 };
            ou_checkbox_group_begin(ctx, "Preferences:");
            ou_checkbox(ctx, "Auto-save", &opts[0]);
            ou_checkbox(ctx, "Spell check", &opts[1]);
            ou_checkbox(ctx, "Line numbers", &opts[2]);
            ou_checkbox_group_end(ctx);
        }

        /* tabs */
        if (ou_section_ex(ctx, "Tabs", OU_HINT_UNFOLDED)) {
            static int tab = 0;
            const char *tabs[] = { "General", "Display", "Audio" };
            ou_tabs(ctx, tabs, 3, &tab);
            if (tab == 0) ou_label(ctx, "General settings here.");
            if (tab == 1) ou_label(ctx, "Display settings here.");
            if (tab == 2) ou_label(ctx, "Audio settings here.");
        }

        /* accordion */
        if (ou_section_ex(ctx, "Accordion", OU_HINT_UNFOLDED)) {
            static int acc = 0;
            if (ou_accordion_item(ctx, "Details", &acc, 0)) {
                ou_label(ctx, "Some detail content.");
            }
            if (ou_accordion_item(ctx, "Settings", &acc, 1)) {
                ou_label(ctx, "Tweak various options.");
            }
            if (ou_accordion_item(ctx, "About", &acc, 2)) {
                ou_label(ctx, "Origami UI v1.0");
            }
        }

        /* collapsible */
        if (ou_section_ex(ctx, "Collapsible", OU_HINT_UNFOLDED)) {
            if (ou_collapsible_begin(ctx, "Click to expand")) {
                ou_label(ctx, "Hidden content revealed!");
                ou_collapsible_end(ctx);
            }
        }

        /* field & fieldset */
        if (ou_section_ex(ctx, "Fieldset", OU_HINT_UNFOLDED)) {
            static char user[64] = "";
            static char pass[64] = "";
            ou_fieldset_begin(ctx, "Login");
            ou_field(ctx, "User:", user, sizeof(user));
            ou_field(ctx, "Pass:", pass, sizeof(pass));
            ou_fieldset_end(ctx);
        }

        /* form */
        if (ou_section_ex(ctx, "Form", OU_HINT_UNFOLDED)) {
            static char fname[64] = "";
            static char femail[64] = "";
            ou_form_begin(ctx, "contact");
            ou_field(ctx, "Name:", fname, sizeof(fname));
            ou_field(ctx, "Email:", femail, sizeof(femail));
            if (ou_form_end(ctx) & OU_SIGNAL_COMMIT) {
                append_log("Form submitted!");
            }
        }

        /* toolbar */
        if (ou_section_ex(ctx, "Toolbar", OU_HINT_UNFOLDED)) {
            ou_toolbar_begin(ctx);
            ou_row(ctx, 4, (int[]) { 0, 0, 0, 0 }, 0);
            if (ou_button(ctx, "New"))  { append_log("New"); }
            if (ou_button(ctx, "Open")) { append_log("Open"); }
            if (ou_button(ctx, "Save")) { append_log("Save"); }
            if (ou_button(ctx, "Exit")) { append_log("Exit"); }
            ou_toolbar_end(ctx);
        }

        /* property editors */
        if (ou_section_ex(ctx, "Property Editors", OU_HINT_UNFOLDED)) {
            static float speed = 1.0f;
            static char  desc[64] = "Hello";
            static int   enabled = 1;
            ou_property_float(ctx, "Speed:", &speed, 0, 10);
            ou_property_text(ctx, "Desc:", desc, sizeof(desc));
            ou_property_bool(ctx, "Enabled", &enabled);
        }

        /* scroll area */
        if (ou_section_ex(ctx, "Scroll Area", OU_HINT_UNFOLDED)) {
            int i;
            ou_scroll_area_begin(ctx, "demo_scroll", 80);
            for (i = 0; i < 20; i++) {
                char buf[32];
                sprintf(buf, "Item %d", i + 1);
                ou_label(ctx, buf);
            }
            ou_scroll_area_end(ctx);
        }

        ou_window_end(ctx);
    }
}


static void overlay_window(ou_context *ctx) {
    if (ou_window_begin(ctx, "Overlay Widgets",
                        ou_rect_make(660, 340, 320, 250)))
    {
        ou_frame *win = ou_active_frame(ctx);
        win->rect.w = ou_max(win->rect.w, 280);

        /* dialog */
        if (ou_section_ex(ctx, "Dialog", OU_HINT_UNFOLDED)) {
            static int dlg_open = 0;
            if (ou_button(ctx, "Open Dialog")) { dlg_open = 1; }
            if (ou_dialog_begin(ctx, "Example Dialog", &dlg_open)) {
                ou_label(ctx, "This is a dialog window.");
                ou_label(ctx, "It renders over a backdrop.");
                ou_separator(ctx);
                ou_row(ctx, 2, (int[]) { 0, 0 }, 0);
                if (ou_button(ctx, "OK")) {
                    dlg_open = 0;
                    append_log("Dialog: OK");
                }
                if (ou_button(ctx, "Cancel")) {
                    dlg_open = 0;
                }
                ou_dialog_end(ctx);
            }
        }

        /* alert dialog */
        if (ou_section_ex(ctx, "Alert Dialog", OU_HINT_UNFOLDED)) {
            static int alert_open = 0;
            if (ou_button(ctx, "Delete Item")) { alert_open = 1; }
            if (ou_alert_dialog(ctx, "Confirm Delete",
                                "Are you sure you want to delete this item? "
                                "This action cannot be undone.",
                                &alert_open) & OU_SIGNAL_COMMIT)
            {
                append_log("Alert: Confirmed delete");
            }
        }

        /* popover */
        if (ou_section_ex(ctx, "Popover", OU_HINT_UNFOLDED)) {
            if (ou_button(ctx, "Show Popover")) {
                ou_summon_popover(ctx, "demo_popover");
            }
            if (ou_popover_begin(ctx, "demo_popover")) {
                ou_label(ctx, "Popover content");
                ou_label(ctx, "anchored below trigger");
                ou_popover_end(ctx);
            }
        }

        /* tooltip */
        if (ou_section_ex(ctx, "Tooltip", OU_HINT_UNFOLDED)) {
            ou_button(ctx, "Hover me");
            ou_tooltip(ctx, "This is a tooltip!");
        }

        /* context menu */
        if (ou_section_ex(ctx, "Context Menu", OU_HINT_UNFOLDED)) {
            ou_label(ctx, "Right-click here:");
            ou_rect r = ou_allocate(ctx);
            ou_paint_rect(ctx, r, ctx->theme->colors[OU_TINT_FIELD]);
            ou_inscribe(ctx, "Right-click zone", r, OU_TINT_LABEL,
                        OU_HINT_CENTER);
            if (ou_context_menu_begin(ctx, "demo_ctx_menu")) {
                if (ou_button(ctx, "Cut"))   { append_log("Cut"); }
                if (ou_button(ctx, "Copy"))  { append_log("Copy"); }
                if (ou_button(ctx, "Paste")) { append_log("Paste"); }
                ou_context_menu_end(ctx);
            }
        }

        /* select */
        if (ou_section_ex(ctx, "Select", OU_HINT_UNFOLDED)) {
            static int sel = 0;
            static const char *options[] = {
                "Apple", "Banana", "Cherry", "Date"
            };
            ou_row(ctx, 2, (int[]) { 50, -1 }, 0);
            ou_label(ctx, "Fruit:");
            if (ou_select(ctx, options, 4, &sel) & OU_SIGNAL_ALTERED) {
                append_log(options[sel]);
            }
        }

        /* preview card */
        if (ou_section_ex(ctx, "Preview Card", OU_HINT_UNFOLDED)) {
            ou_button(ctx, "Hover for preview");
            if (ou_preview_card_begin(ctx, "demo_preview")) {
                ou_label(ctx, "Preview Card");
                ou_separator(ctx);
                ou_label(ctx, "Shows on hover with");
                ou_label(ctx, "rich content inside.");
                ou_preview_card_end(ctx);
            }
        }

        ou_window_end(ctx);
    }
}


static void build_ui(ou_context *ctx) {
    ou_frame_begin(ctx);
    palette_window(ctx);
    journal_window(ctx);
    showcase_window(ctx);
    compound_window(ctx);
    overlay_window(ctx);
    ou_frame_end(ctx);
}


static const char button_map[256] = {
    [SDL_BUTTON_LEFT   & 0xff] = OU_MOUSE_PRIMARY,
    [SDL_BUTTON_RIGHT  & 0xff] = OU_MOUSE_SECONDARY,
    [SDL_BUTTON_MIDDLE & 0xff] = OU_MOUSE_TERTIARY,
};

static const char key_map[256] = {
    [SDLK_LSHIFT    & 0xff] = OU_KEY_SHIFT,
    [SDLK_RSHIFT    & 0xff] = OU_KEY_SHIFT,
    [SDLK_LCTRL     & 0xff] = OU_KEY_CTRL,
    [SDLK_RCTRL     & 0xff] = OU_KEY_CTRL,
    [SDLK_LALT      & 0xff] = OU_KEY_ALT,
    [SDLK_RALT      & 0xff] = OU_KEY_ALT,
    [SDLK_RETURN    & 0xff] = OU_KEY_ENTER,
    [SDLK_BACKSPACE & 0xff] = OU_KEY_ERASE,
};


static int measure_width(ou_font font, const char *text, int len) {
    (void) font;
    if (len == -1) { len = (int) strlen(text); }
    return r_get_text_width(text, len);
}

static int measure_height(ou_font font) {
    (void) font;
    return r_get_text_height();
}


int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    SDL_Init(SDL_INIT_EVERYTHING);
    r_init();

    ou_context *ctx = malloc(sizeof(ou_context));
    ou_setup(ctx);
    ctx->text_width = measure_width;
    ctx->text_height = measure_height;

    for (;;) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            case SDL_MOUSEMOTION:
                ou_feed_motion(ctx, e.motion.x, e.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                ou_feed_scroll(ctx, 0, e.wheel.y * -30);
                break;
            case SDL_TEXTINPUT:
                ou_feed_text(ctx, e.text.text);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                int b = button_map[e.button.button & 0xff];
                if (b && e.type == SDL_MOUSEBUTTONDOWN) {
                    ou_feed_press(ctx, e.button.x, e.button.y, b);
                }
                if (b && e.type == SDL_MOUSEBUTTONUP) {
                    ou_feed_release(ctx, e.button.x, e.button.y, b);
                }
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                int c = key_map[e.key.keysym.sym & 0xff];
                if (c && e.type == SDL_KEYDOWN) { ou_feed_keydown(ctx, c); }
                if (c && e.type == SDL_KEYUP)   { ou_feed_keyup(ctx, c);   }
                break;
            }
            }
        }

        build_ui(ctx);

        r_clear(ou_color_make((int) bg[0], (int) bg[1], (int) bg[2], 255));
        ou_cmd *cmd = NULL;
        while (ou_walk(ctx, &cmd)) {
            switch (cmd->type) {
            case OU_CMD_GLYPH:
                r_draw_text(cmd->glyph.str, cmd->glyph.pos, cmd->glyph.color);
                break;
            case OU_CMD_FILL:
                r_draw_rect(cmd->fill.rect, cmd->fill.color);
                break;
            case OU_CMD_SYMBOL:
                r_draw_icon(cmd->symbol.id, cmd->symbol.rect,
                            cmd->symbol.color);
                break;
            case OU_CMD_SCISSOR:
                r_set_clip_rect(cmd->scissor.rect);
                break;
            }
        }
        r_present();
    }

    return 0;
}
