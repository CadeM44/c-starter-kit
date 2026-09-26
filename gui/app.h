#ifndef APP_H
#define APP_H

#include <stddef.h>

#define APP_NAME_MAX 63
#define APP_PATH_MAX 511
#define APP_STATUS_MAX 1023
#define APP_ROOT_MAX 511
#define APP_PREVIEW_MAX 511

typedef enum App_Page
{
    App_Page_Generate,
    App_Page_COUNT
}
App_Page;

typedef enum App_Focus
{
    App_Focus_None,
    App_Focus_Name,
    App_Focus_Output,
    App_Focus_COUNT
}
App_Focus;

typedef enum App_Click
{
    App_Click_None = 0,
    App_Click_FocusName,
    App_Click_FocusOutput,
    App_Click_Generate,
    App_Click_PageGenerate,
    App_Click_TemplateBase = 100
}
App_Click;

typedef struct App_Template
{
    const char *id;
    const char *standard;
    const char *title;
    const char *blurb;
    const char *preview;
}
App_Template;

typedef struct App
{
    char name[APP_NAME_MAX + 1];
    char output[APP_PATH_MAX + 1];
    char status[APP_STATUS_MAX + 1];
    char root[APP_ROOT_MAX + 1];
    char preview[APP_PREVIEW_MAX + 1];
    int status_ok;
    int template_index;
    App_Page page;
    App_Focus focus;
}
App;

void app_init(const char *argv0);
App *app_get(void);

const App_Template *app_templates(void);
int app_template_count(void);
const App_Template *app_selected_template(void);

void app_set_status(int ok, const char *msg);
void app_set_page(App_Page page);
void app_set_focus(App_Focus focus);
void app_select_template(int index);
void app_refresh_preview(void);

void app_handle_keys(void);
void app_handle_click(int click);
void app_generate(void);

#endif
