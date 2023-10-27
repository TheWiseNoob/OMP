#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"

struct _ExampleApp
{
  AdwApplication parent;
};

G_DEFINE_TYPE(ExampleApp, example_app, ADW_TYPE_APPLICATION);

static void
example_app_init (ExampleApp *app)
{
}

static void
example_app_activate (GApplication *app)
{
  ExampleAppWindow *win;

  win = example_app_window_new (EXAMPLE_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
example_app_open (GApplication  *app,
                  GFile        **files,
                  int            n_files,
                  const char    *hint)
{
  GList *windows;
  ExampleAppWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = EXAMPLE_APP_WINDOW (windows->data);
  else
    win = example_app_window_new (EXAMPLE_APP (app));

  for (i = 0; i < n_files; i++)
    example_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

static void
example_app_class_init (ExampleAppClass *self)
{
  G_APPLICATION_CLASS (self)->activate = example_app_activate;
  G_APPLICATION_CLASS (self)->open = example_app_open;
}

ExampleApp *
example_app_new (void)
{
  return (ExampleApp*)g_object_new (EXAMPLE_APP_TYPE,
                       "application-id", "org.gtk.omp",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}
