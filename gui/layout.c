#include "layout.h"

#include <stdint.h>
#include <string.h>

#include "raylib.h"

#include "app.h"
#include "theme.h"

static void on_click(Clay_ElementId id, Clay_PointerData pointer, void *user)
{
    (void)id;
    if (pointer.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME)
    {
        return;
    }
    app_handle_click((int)(intptr_t)user);
}

static Clay_String dyn_str(const char *s)
{
    if (!s)
    {
        s = "";
    }
    return (Clay_String) {
        .isStaticallyAllocated = false,
        .length = (int32_t)strlen(s),
        .chars = s,
    };
}

static Clay_TextElementConfig text_cfg(uint16_t size, Clay_Color color)
{
    return (Clay_TextElementConfig) {
        .textColor = color,
        .fontId = UI_FONT_BODY,
        .fontSize = size,
        .lineHeight = (uint16_t)(size + 6),
    };
}

static Clay_TextElementConfig text_pre(uint16_t size, Clay_Color color)
{
    Clay_TextElementConfig cfg = text_cfg(size, color);
    cfg.wrapMode = CLAY_TEXT_WRAP_NEWLINES;
    cfg.lineHeight = (uint16_t)(size + 8);
    return cfg;
}

static void rainbow_stripe(void)
{
    CLAY(CLAY_ID("Stripe"), {
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(UI_STRIPE_HEIGHT) },
        },
    })
    {
        for (int i = 0; i < UI_STRIPE_COUNT; i++)
        {
            CLAY(CLAY_IDI("stripe_cell", i), {
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                },
                .backgroundColor = UI_Color_Stripe[i],
            })
            {
            }
        }
    }
}

static void labeled_field(int index, const char *label, const char *value, int focused, App_Click click)
{
    Clay_Color border = focused ? UI_Color_Blue : UI_Color_Line;
    int caret_on = focused && ((int)(GetTime() * 2.0) % 2 == 0);

    CLAY(CLAY_IDI("field", index), {
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = { .width = CLAY_SIZING_GROW(0) },
            .childGap = UI_SPACE_XS,
        },
    })
    {
        CLAY_TEXT(dyn_str(label), text_cfg(12, UI_Color_Muted));
        CLAY(CLAY_IDI("box", index), {
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(UI_FIELD_HEIGHT) },
                .padding = { 12, 12, 10, 10 },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                .childGap = 2,
            },
            .backgroundColor = UI_Color_Field,
            .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_SM),
            .border = { .width = { 1, 1, 1, 1 }, .color = border },
        })
        {
            Clay_OnHover(on_click, (void *)(intptr_t)click);
            const char *shown = value[0] ? value : (focused ? "" : "...");
            Clay_Color col = value[0] ? UI_Color_Text : UI_Color_Muted;
            CLAY_TEXT(dyn_str(shown), text_cfg(16, col));
            if (caret_on)
            {
                CLAY(CLAY_IDI("caret", index), {
                    .layout = {
                        .sizing = { .width = CLAY_SIZING_FIXED(2), .height = CLAY_SIZING_FIXED(18) },
                    },
                    .backgroundColor = UI_Color_Blue,
                })
                {
                }
            }
        }
    }
}

static void template_card(int index, const App_Template *t, int selected)
{
    Clay_Color fill = selected ? UI_Color_CardOn : (Clay_Hovered() ? UI_Color_CardHot : UI_Color_Card);
    Clay_Color border = selected ? UI_Color_Stripe[index % UI_STRIPE_COUNT] : UI_Color_Line;

    CLAY(CLAY_IDI("card", index), {
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0) },
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
        },
        .backgroundColor = fill,
        .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_MD),
        .border = { .width = { 1, 1, 1, 1 }, .color = border },
    })
    {
        Clay_OnHover(on_click, (void *)(intptr_t)(App_Click_TemplateBase + index));
        CLAY(CLAY_IDI("card_mark", index), {
            .layout = {
                .sizing = { .width = CLAY_SIZING_FIXED(6), .height = CLAY_SIZING_GROW(0) },
            },
            .backgroundColor = selected ? UI_Color_Stripe[index % UI_STRIPE_COUNT] : UI_COL_A(0, 0, 0, 0),
            .cornerRadius = (Clay_CornerRadius) { UI_RADIUS_MD, 0, UI_RADIUS_MD, 0 },
        })
        {
        }
        CLAY(CLAY_IDI("card_body", index), {
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = { .width = CLAY_SIZING_GROW(0) },
                .padding = { 12, 14, 12, 12 },
                .childGap = 4,
            },
        })
        {
            CLAY_TEXT(dyn_str(t->title), text_cfg(16, UI_Color_Text));
            CLAY_TEXT(dyn_str(t->blurb), text_cfg(13, UI_Color_Muted));
        }
    }
}

static void sidebar_item(int index, const char *label, int selected, App_Click click)
{
    Clay_Color fill = selected ? UI_Color_CardOn : (Clay_Hovered() ? UI_Color_CardHot : UI_COL_A(0, 0, 0, 0));
    Clay_Color text = selected ? UI_Color_Text : UI_Color_Muted;

    CLAY(CLAY_IDI("nav", index), {
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(36) },
            .padding = { 10, 10, 8, 8 },
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 10,
        },
        .backgroundColor = fill,
        .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_SM),
    })
    {
        Clay_OnHover(on_click, (void *)(intptr_t)click);
        CLAY(CLAY_IDI("nav_dot", index), {
            .layout = {
                .sizing = { .width = CLAY_SIZING_FIXED(8), .height = CLAY_SIZING_FIXED(8) },
            },
            .backgroundColor = selected ? UI_Color_Green : UI_Color_Line,
            .cornerRadius = CLAY_CORNER_RADIUS(4),
        })
        {
        }
        CLAY_TEXT(dyn_str(label), text_cfg(14, text));
    }
}

static void generate_page(const App *app)
{
    CLAY(CLAY_ID("GenerateRow"), {
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
            .childGap = UI_SPACE_LG,
        },
    })
    {
        CLAY(CLAY_ID("Form"), {
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                .childGap = UI_SPACE_MD,
            },
        })
        {
            labeled_field(0, "PROJECT NAME", app->name, app->focus == App_Focus_Name, App_Click_FocusName);
            labeled_field(1, "OUTPUT DIRECTORY", app->output, app->focus == App_Focus_Output, App_Click_FocusOutput);

            CLAY_TEXT(CLAY_STRING("TEMPLATE"), text_cfg(12, UI_Color_Muted));
            CLAY(CLAY_ID("Cards"), {
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = { .width = CLAY_SIZING_GROW(0) },
                    .childGap = UI_SPACE_SM,
                },
            })
            {
                const App_Template *templates = app_templates();
                int count = app_template_count();
                for (int i = 0; i < count; i++)
                {
                    template_card(i, &templates[i], app->template_index == i);
                }
            }

            CLAY(CLAY_ID("Actions"), {
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW(0) },
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .childGap = 14,
                },
            })
            {
                Clay_Color btn = Clay_Hovered() ? UI_Color_BlueHot : UI_Color_Blue;
                CLAY(CLAY_ID("Generate"), {
                    .layout = {
                        .padding = { 18, 18, 12, 12 },
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                    },
                    .backgroundColor = btn,
                    .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_PILL),
                })
                {
                    Clay_OnHover(on_click, (void *)(intptr_t)App_Click_Generate);
                    CLAY_TEXT(CLAY_STRING("Generate project"), text_cfg(15, UI_Color_BtnText));
                }
                CLAY_TEXT(CLAY_STRING("Enter  ·  Tab fields  ·  1-2 standard"), text_cfg(12, UI_Color_Muted));
            }
        }

        CLAY(CLAY_ID("Aside"), {
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = { .width = CLAY_SIZING_FIXED(280), .height = CLAY_SIZING_GROW(0) },
                .childGap = UI_SPACE_MD,
            },
        })
        {
            CLAY(CLAY_ID("Preview"), {
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                    .padding = CLAY_PADDING_ALL(14),
                    .childGap = UI_SPACE_SM,
                },
                .backgroundColor = UI_Color_Card,
                .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_MD),
                .border = { .width = { 1, 1, 1, 1 }, .color = UI_Color_Line },
            })
            {
                CLAY_TEXT(CLAY_STRING("PREVIEW"), text_cfg(12, UI_Color_Muted));
                CLAY_TEXT(dyn_str(app->preview), text_pre(13, UI_Color_Text));
            }

            CLAY(CLAY_ID("Log"), {
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(140) },
                    .padding = CLAY_PADDING_ALL(14),
                    .childGap = UI_SPACE_XS,
                },
                .backgroundColor = UI_Color_Card,
                .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_MD),
                .border = { .width = { 1, 1, 1, 1 }, .color = UI_Color_Line },
                .clip = { .horizontal = false, .vertical = true },
            })
            {
                CLAY_TEXT(CLAY_STRING("STATUS"), text_cfg(12, UI_Color_Muted));
                CLAY_TEXT(dyn_str(app->status), text_cfg(14, app->status_ok ? UI_Color_Ok : UI_Color_Err));
            }
        }
    }
}

Clay_RenderCommandArray layout_build(void)
{
    const App *app = app_get();
    Clay_BeginLayout();

    CLAY(CLAY_ID("Root"), {
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
        },
        .backgroundColor = UI_Color_Bg,
    })
    {
        rainbow_stripe();

        CLAY(CLAY_ID("TitleBar"), {
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(UI_TITLEBAR_HEIGHT) },
                .padding = { UI_SPACE_XL, UI_SPACE_XL, 0, 0 },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
        })
        {
            CLAY(CLAY_ID("TitleCol"), {
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = { .width = CLAY_SIZING_GROW(0) },
                    .childGap = 2,
                },
            })
            {
                CLAY_TEXT(CLAY_STRING("c-starter-kit"), text_cfg(22, UI_Color_Text));
                CLAY_TEXT(CLAY_STRING("C99 / C23 project skeletons"), text_cfg(13, UI_Color_Muted));
            }
        }

        CLAY(CLAY_ID("Body"), {
            .layout = {
                .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                .padding = { UI_SPACE_XL, UI_SPACE_XL, 8, UI_SPACE_XL },
                .childGap = UI_SPACE_LG,
            },
        })
        {
            CLAY(CLAY_ID("Sidebar"), {
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = { .width = CLAY_SIZING_FIXED(UI_SIDEBAR_WIDTH), .height = CLAY_SIZING_GROW(0) },
                    .padding = CLAY_PADDING_ALL(12),
                    .childGap = 6,
                },
                .backgroundColor = UI_Color_Panel,
                .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_LG),
                .border = { .width = { 1, 1, 1, 1 }, .color = UI_Color_Line },
            })
            {
                CLAY_TEXT(CLAY_STRING("WORKSPACE"), text_cfg(11, UI_Color_Muted));
                sidebar_item(0, "New project", app->page == App_Page_Generate, App_Click_PageGenerate);

                CLAY(CLAY_ID("SidebarSpacer"), {
                    .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } },
                })
                {
                }

                CLAY_TEXT(CLAY_STRING("KIT ROOT"), text_cfg(11, UI_Color_Muted));
                CLAY_TEXT(dyn_str(app->root), text_cfg(12, UI_Color_Text));
            }

            CLAY(CLAY_ID("Panel"), {
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
                    .padding = CLAY_PADDING_ALL(20),
                    .childGap = UI_SPACE_MD,
                },
                .backgroundColor = UI_Color_Panel,
                .cornerRadius = CLAY_CORNER_RADIUS(UI_RADIUS_LG),
                .border = { .width = { 1, 1, 1, 1 }, .color = UI_Color_Line },
            })
            {
                switch (app->page)
                {
                case App_Page_Generate:
                case App_Page_COUNT:
                    generate_page(app);
                    break;
                }
            }
        }
    }

    return Clay_EndLayout(GetFrameTime());
}
