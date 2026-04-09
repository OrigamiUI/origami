# Unity Test Inventory - Origami UI (Assignment 3 Submission)

This document lists the full automated Unity suite executed by `tests/run_tests.ps1`.

- Component tests: 29
- Integration tests: 3
- Total: 32
- Latest recorded result: `32 Tests 0 Failures 0 Ignored` (see [test_logs/tc04_tc06_unity_suite.log](test_logs/tc04_tc06_unity_suite.log))

---

## Component Tests (29)


| #   | Component Source                  | Test File                                | Test Function                                            |
| --- | --------------------------------- | ---------------------------------------- | -------------------------------------------------------- |
| 1   | `src/components/accordion.c`      | `tests/components/test_accordion.c`      | `test_accordion_item_reports_active_when_selected`       |
| 2   | `src/components/avatar.c`         | `tests/components/test_avatar.c`         | `test_avatar_ex_emits_draw_commands`                     |
| 3   | `src/components/button.c`         | `tests/components/test_button.c`         | `test_button_ex_returns_zero_without_press`              |
| 4   | `src/components/checkbox.c`       | `tests/components/test_checkbox.c`       | `test_checkbox_does_not_toggle_without_press`            |
| 5   | `src/components/checkbox_group.c` | `tests/components/test_checkbox_group.c` | `test_checkbox_group_begin_end_restore_scope_and_indent` |
| 6   | `src/components/collapsible.c`    | `tests/components/test_collapsible.c`    | `test_section_ex_unfolded_is_active`                     |
| 7   | `src/components/context_menu.c`   | `tests/components/test_context_menu.c`   | `test_context_menu_begin_after_open_is_active`           |
| 8   | `src/components/dialog.c`         | `tests/components/test_dialog.c`         | `test_dialog_begin_respects_open_flag`                   |
| 9   | `src/components/field.c`          | `tests/components/test_field.c`          | `test_fieldset_and_form_balance_scope`                   |
| 10  | `src/components/label.c`          | `tests/components/test_label.c`          | `test_paragraph_and_label_emit_commands`                 |
| 11  | `src/components/panel.c`          | `tests/components/test_panel.c`          | `test_panel_begin_end_balances_frame_stack`              |
| 12  | `src/components/popover.c`        | `tests/components/test_popover.c`        | `test_popover_begin_after_summon_is_active`              |
| 13  | `src/components/popup.c`          | `tests/components/test_popup.c`          | `test_popup_begin_after_summon_is_active`                |
| 14  | `src/components/preview_card.c`   | `tests/components/test_preview_card.c`   | `test_preview_card_begin_when_hovered_is_active`         |
| 15  | `src/components/progress.c`       | `tests/components/test_progress.c`       | `test_progress_and_meter_emit_commands`                  |
| 16  | `src/components/radio.c`          | `tests/components/test_radio.c`          | `test_radio_does_not_change_selection_without_press`     |
| 17  | `src/components/scroll_area.c`    | `tests/components/test_scroll_area.c`    | `test_scroll_area_begin_end_balances_frame_stack`        |
| 18  | `src/components/select.c`         | `tests/components/test_select.c`         | `test_select_returns_zero_without_click`                 |
| 19  | `src/components/separator.c`      | `tests/components/test_separator.c`      | `test_separator_emits_line_command`                      |
| 20  | `src/components/slider.c`         | `tests/components/test_slider.c`         | `test_slider_clamps_value_to_range`                      |
| 21  | `src/components/switch.c`         | `tests/components/test_switch.c`         | `test_switch_does_not_toggle_without_press`              |
| 22  | `src/components/tabs.c`           | `tests/components/test_tabs.c`           | `test_tabs_without_press_keep_selection`                 |
| 23  | `src/components/textfield.c`      | `tests/components/test_textfield.c`      | `test_textfield_raw_appends_input_when_focused`          |
| 24  | `src/components/theme_builder.c`  | `tests/components/test_theme_builder.c`  | `test_theme_builder_applies_mutations`                   |
| 25  | `src/components/toggle.c`         | `tests/components/test_toggle.c`         | `test_toggle_does_not_flip_without_press`                |
| 26  | `src/components/toggle_group.c`   | `tests/components/test_toggle_group.c`   | `test_toggle_group_without_press_keeps_selection`        |
| 27  | `src/components/toolbar.c`        | `tests/components/test_toolbar.c`        | `test_toolbar_begin_end_balances_grid_depth`             |
| 28  | `src/components/tooltip.c`        | `tests/components/test_tooltip.c`        | `test_tooltip_emits_overlay_when_anchor_is_hovered`      |
| 29  | `src/components/window.c`         | `tests/components/test_window.c`         | `test_window_begin_end_works_standalone`                 |


---

## Integration Tests (3)


| #   | Test File                  | Test Function                                              | Purpose                                    |
| --- | -------------------------- | ---------------------------------------------------------- | ------------------------------------------ |
| 1   | `tests/test_integration.c` | `test_command_pipeline_walk_returns_draw_commands`         | Verifies command pipeline walk/render flow |
| 2   | `tests/test_integration.c` | `test_textfield_enter_returns_commit_and_clears_focus`     | Verifies Enter commit behavior             |
| 3   | `tests/test_integration.c` | `test_theme_change_affects_rendered_fill_color_next_frame` | Verifies runtime theme update propagation  |


---

## Notes

- Assignment-level test cases in [test_report_v1.md](test_report_v1.md) are scenario-oriented (`TC-01..TC-07`).
- This document is the detailed automated inventory behind `TC-04` and `TC-06`.
