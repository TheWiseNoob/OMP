#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"
#include "content.h"
#include "sidebar.h"

struct _OMPApp {
    AdwApplication parent;

    OMPContent* content;
    OMPSidebar* sidebar;
    OMPAppWindow* main_window;
};
G_DEFINE_TYPE (OMPApp, omp_app, ADW_TYPE_APPLICATION);

static void
omp_app_activate (GApplication* app)
{
    OMPAppWindow* win;

    OMPApp* omp_app = (OMPApp*)(app);
    win = omp_app_window_new (omp_app);
    omp_app->main_window = win;
    gtk_window_present (GTK_WINDOW (omp_app->main_window));
}

static void
omp_app_open (GApplication* app, GFile** files, int n_files, const char* hint)
{
    GList* windows;
    OMPAppWindow* win;
    int i;

    windows = gtk_application_get_windows (GTK_APPLICATION (app));
    if (windows) {
        win = OMP_APP_WINDOW (windows->data);
    }
    else {
        win = omp_app_window_new (OMP_APP (app));
    }

    for (i = 0; i < n_files; i++)
        omp_app_window_open (win, files[i]);

    gtk_window_present (GTK_WINDOW (win));
}

//
// Inits
//
static void
omp_app_init (OMPApp* app)
{
}

static void
omp_app_class_init (OMPAppClass* self)
{
    G_APPLICATION_CLASS (self)->activate = omp_app_activate;
    G_APPLICATION_CLASS (self)->open = omp_app_open;
}

//
// Member Functions
//
OMPApp*
omp_app_new (void)
{
    return (OMPApp*)g_object_new (
        OMP_APP_TYPE, "application-id", "com.openmusicplayer.omp", "flags",
        G_APPLICATION_HANDLES_OPEN, NULL
    );
}

OMPAppWindow*
omp_app_get_main_window (OMPApp* app)
{
    return (OMPAppWindow*)app->main_window;
}

OMPContent*
omp_app_get_content (OMPApp* app)
{
    return app->content;
}

void
omp_app_set_content (OMPApp* app, OMPContent* content)
{
    app->content = content;
}

OMPSidebar*
omp_app_get_sidebar (OMPApp* app)
{
    return app->sidebar;
}

void
omp_app_set_sidebar (OMPApp* app, OMPSidebar* sidebar)
{
    app->sidebar = sidebar;
}
