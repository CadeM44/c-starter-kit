#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_renderer_raylib.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "app.h"
#include "layout.h"
#include "theme.h"

static int file_exists(const char *path)
{
    return path && path[0] && access(path, R_OK) == 0;
}

static void handle_clay_errors(Clay_ErrorData error_data)
{
    fprintf(stderr, "clay: %.*s\n", error_data.errorText.length, error_data.errorText.chars);
}

static Font load_ui_font(void)
{
    static const char *paths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "C:\\Windows\\Fonts\\segoeui.ttf",
        NULL,
    };

    for (int i = 0; paths[i]; i++)
    {
        if (!file_exists(paths[i]))
        {
            continue;
        }
        Font font = LoadFontEx(paths[i], 32, NULL, 0);
        if (font.glyphCount > 0)
        {
            SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
            return font;
        }
    }
    return GetFontDefault();
}

int main(int argc, char **argv)
{
    app_init(argc > 0 ? argv[0] : NULL);

    Clay_Raylib_Initialize(980, 720, "c-starter-kit",
                           FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    uint32_t memory_size = Clay_MinMemorySize();
    void *memory = malloc(memory_size);
    if (!memory)
    {
        fprintf(stderr, "out of memory for clay arena\n");
        return 1;
    }

    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(memory_size, memory);
    Clay_Initialize(arena,
                    (Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() },
                    (Clay_ErrorHandler) { handle_clay_errors, 0 });

    Font fonts[UI_FONT_COUNT];
    fonts[UI_FONT_BODY] = load_ui_font();
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        Clay_SetPointerState((Clay_Vector2) { mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));
        Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });
        Clay_UpdateScrollContainers(true, (Clay_Vector2) { 0, GetMouseWheelMove() * 24.0f }, GetFrameTime());

        app_handle_keys();

        Clay_RenderCommandArray commands = layout_build();

        BeginDrawing();
        Color bg = {
            (unsigned char)UI_Color_Bg.r,
            (unsigned char)UI_Color_Bg.g,
            (unsigned char)UI_Color_Bg.b,
            255,
        };
        ClearBackground(bg);
        Clay_Raylib_Render(commands, fonts);
        EndDrawing();
    }

    UnloadFont(fonts[UI_FONT_BODY]);
    Clay_Raylib_Close();
    free(memory);
    return 0;
}
