#include "app.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "raylib.h"

static App g_app;

static const App_Template g_templates[] = {
    {
        "c99-starter",
        "c99",
        "C99  ·  CMake",
        "src + include, CMake 3.16+",
        "%s/\n"
        "  CMakeLists.txt\n"
        "  src/%s.c\n"
        "  src/main.c\n"
        "  include/%s/%s.h\n"
        "  tests/test_%s.c\n"
        "  .editorconfig\n"
        "  .clang-format",
    },
    {
        "c2x-starter",
        "c23",
        "C23  ·  CMake",
        "same layout, c_std_23 / MSVC C23",
        "%s/\n"
        "  CMakeLists.txt\n"
        "  src/%s.c\n"
        "  src/main.c\n"
        "  include/%s/%s.h\n"
        "  tests/test_%s.c\n"
        "  .editorconfig\n"
        "  .clang-format",
    },
};

static const int g_template_count = (int)(sizeof g_templates / sizeof g_templates[0]);

static void join_path(char *dst, size_t cap, const char *a, const char *b)
{
    size_t n = strlen(a);
    while (n > 0 && a[n - 1] == '/')
    {
        n--;
    }
    snprintf(dst, cap, "%.*s/%s", (int)n, a, b);
}

static void dirname_of(char *dst, size_t cap, const char *path)
{
    const char *slash = strrchr(path, '/');
    if (!slash)
    {
        snprintf(dst, cap, ".");
        return;
    }
    if (slash == path)
    {
        snprintf(dst, cap, "/");
        return;
    }

    size_t n = (size_t)(slash - path);
    if (n >= cap)
    {
        n = cap - 1;
    }
    memcpy(dst, path, n);
    dst[n] = 0;
}

static int file_exists(const char *path)
{
    return path && path[0] && access(path, R_OK) == 0;
}

static int look_for_kit(const char *dir)
{
    char probe[APP_ROOT_MAX + 64];
    join_path(probe, sizeof probe, dir, "cstarter.sh");
    if (file_exists(probe))
    {
        snprintf(g_app.root, sizeof g_app.root, "%s", dir);
        return 1;
    }
    return 0;
}

static void discover_root(const char *argv0)
{
    const char *env = getenv("CSTARTER_ROOT");
    if (env && look_for_kit(env))
    {
        return;
    }

    char cwd[APP_ROOT_MAX + 1];
    if (getcwd(cwd, sizeof cwd))
    {
        if (look_for_kit(cwd))
        {
            return;
        }
        char parent[APP_ROOT_MAX + 1];
        dirname_of(parent, sizeof parent, cwd);
        if (look_for_kit(parent))
        {
            return;
        }
        char grand[APP_ROOT_MAX + 1];
        dirname_of(grand, sizeof grand, parent);
        if (look_for_kit(grand))
        {
            return;
        }
    }

    if (argv0 && argv0[0])
    {
        char abs[APP_ROOT_MAX + 1];
        if (argv0[0] == '/')
        {
            snprintf(abs, sizeof abs, "%s", argv0);
        }
        else if (getcwd(cwd, sizeof cwd))
        {
            join_path(abs, sizeof abs, cwd, argv0);
        }
        else
        {
            abs[0] = 0;
        }

        if (abs[0])
        {
            char dir[APP_ROOT_MAX + 1];
            dirname_of(dir, sizeof dir, abs);
            if (look_for_kit(dir))
            {
                return;
            }
            char parent[APP_ROOT_MAX + 1];
            dirname_of(parent, sizeof parent, dir);
            if (look_for_kit(parent))
            {
                return;
            }
            char grand[APP_ROOT_MAX + 1];
            dirname_of(grand, sizeof grand, parent);
            if (look_for_kit(grand))
            {
                return;
            }
        }
    }

    snprintf(g_app.root, sizeof g_app.root, ".");
}

static int is_ident(const char *s)
{
    if (!s || !s[0])
    {
        return 0;
    }
    if (!(isalpha((unsigned char)s[0]) || s[0] == '_'))
    {
        return 0;
    }
    for (const char *p = s + 1; *p; p++)
    {
        if (!(isalnum((unsigned char)*p) || *p == '_'))
        {
            return 0;
        }
    }
    return 1;
}

static void append_char(char *buf, size_t cap, int ch)
{
    size_t n = strlen(buf);
    if (n + 1 >= cap)
    {
        return;
    }
    buf[n] = (char)ch;
    buf[n + 1] = 0;
}

static void backspace(char *buf)
{
    size_t n = strlen(buf);
    if (n > 0)
    {
        buf[n - 1] = 0;
    }
}

void app_set_status(int ok, const char *msg)
{
    g_app.status_ok = ok;
    snprintf(g_app.status, sizeof g_app.status, "%s", msg ? msg : "");
}

void app_refresh_preview(void)
{
    const App_Template *t = app_selected_template();
    const char *name = g_app.name[0] ? g_app.name : "myproj";
    snprintf(g_app.preview, sizeof g_app.preview, t->preview, name, name, name, name, name);
}

void app_init(const char *argv0)
{
    memset(&g_app, 0, sizeof g_app);
    snprintf(g_app.output, sizeof g_app.output, ".");
    g_app.status_ok = 1;
    g_app.page = App_Page_Generate;
    g_app.focus = App_Focus_Name;
    discover_root(argv0);
    app_refresh_preview();

    char hello[APP_STATUS_MAX + 1];
    snprintf(hello, sizeof hello, "Ready. Kit root: %s", g_app.root);
    app_set_status(1, hello);
}

App *app_get(void)
{
    return &g_app;
}

const App_Template *app_templates(void)
{
    return g_templates;
}

int app_template_count(void)
{
    return g_template_count;
}

const App_Template *app_selected_template(void)
{
    return &g_templates[g_app.template_index];
}

void app_set_page(App_Page page)
{
    if (page >= 0 && page < App_Page_COUNT)
    {
        g_app.page = page;
    }
}

void app_set_focus(App_Focus focus)
{
    g_app.focus = focus;
}

void app_select_template(int index)
{
    if (index < 0 || index >= g_template_count)
    {
        return;
    }
    g_app.template_index = index;
    app_refresh_preview();
}

void app_handle_click(int click)
{
    if (click >= App_Click_TemplateBase)
    {
        app_select_template(click - App_Click_TemplateBase);
        return;
    }

    switch (click)
    {
    case App_Click_FocusName:
        g_app.focus = App_Focus_Name;
        break;
    case App_Click_FocusOutput:
        g_app.focus = App_Focus_Output;
        break;
    case App_Click_Generate:
        app_generate();
        break;
    case App_Click_PageGenerate:
        g_app.page = App_Page_Generate;
        break;
    default:
        break;
    }
}

void app_handle_keys(void)
{
    int key;
    while ((key = GetCharPressed()) != 0)
    {
        if (key < 32 || key >= 127)
        {
            continue;
        }
        if (g_app.focus == App_Focus_Name)
        {
            if (isalnum(key) || key == '_')
            {
                append_char(g_app.name, sizeof g_app.name, key);
                app_refresh_preview();
            }
        }
        else if (g_app.focus == App_Focus_Output)
        {
            append_char(g_app.output, sizeof g_app.output, key);
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (g_app.focus == App_Focus_Name)
        {
            backspace(g_app.name);
            app_refresh_preview();
        }
        else if (g_app.focus == App_Focus_Output)
        {
            backspace(g_app.output);
        }
    }

    if (IsKeyPressed(KEY_TAB))
    {
        int backward = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        if (backward)
        {
            g_app.focus = (g_app.focus == App_Focus_Output) ? App_Focus_Name : App_Focus_Output;
        }
        else
        {
            g_app.focus = (g_app.focus == App_Focus_Name) ? App_Focus_Output : App_Focus_Name;
        }
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
    {
        app_generate();
    }

    if (IsKeyPressed(KEY_ONE))
    {
        app_select_template(0);
    }
    if (IsKeyPressed(KEY_TWO) && g_template_count > 1)
    {
        app_select_template(1);
    }
}

void app_generate(void)
{
    if (!is_ident(g_app.name))
    {
        app_set_status(0, "Name must be a C identifier: [A-Za-z_][A-Za-z0-9_]*");
        return;
    }

    char script[APP_ROOT_MAX + 32];
    join_path(script, sizeof script, g_app.root, "cstarter.sh");
    if (!file_exists(script))
    {
        char msg[APP_STATUS_MAX + 1];
        snprintf(msg, sizeof msg, "Cannot find cstarter.sh under %s", g_app.root);
        app_set_status(0, msg);
        return;
    }

    const char *out = g_app.output[0] ? g_app.output : ".";
    char cmd[APP_ROOT_MAX + APP_NAME_MAX + APP_PATH_MAX + 128];
    snprintf(cmd, sizeof cmd,
             "bash \"%s\" --name \"%s\" --template \"%s\" --output \"%s\" 2>&1",
             script, g_app.name, app_selected_template()->id, out);

    FILE *pipe = popen(cmd, "r");
    if (!pipe)
    {
        app_set_status(0, strerror(errno));
        return;
    }

    char outbuf[APP_STATUS_MAX + 1];
    size_t used = 0;
    outbuf[0] = 0;
    while (used + 1 < sizeof outbuf)
    {
        size_t n = fread(outbuf + used, 1, sizeof outbuf - 1 - used, pipe);
        if (n == 0)
        {
            break;
        }
        used += n;
        outbuf[used] = 0;
    }

    int rc = pclose(pipe);
    if (used == 0)
    {
        snprintf(outbuf, sizeof outbuf, rc == 0 ? "Created project." : "Generation failed.");
        used = strlen(outbuf);
    }
    while (used > 0 && (outbuf[used - 1] == '\n' || outbuf[used - 1] == '\r' || outbuf[used - 1] == ' '))
    {
        outbuf[--used] = 0;
    }
    app_set_status(rc == 0, outbuf);
}
