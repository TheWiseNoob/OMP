#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"

struct _OMPApp
{
  AdwApplication parent;
};

G_DEFINE_TYPE(OMPApp, omp_app, ADW_TYPE_APPLICATION);

static void
omp_app_init (OMPApp *app)
{
}

static void
omp_app_activate (GApplication *app)
{
  OMPAppWindow *win;

  win = omp_app_window_new (OMP_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
omp_app_open (GApplication  *app,
                  GFile        **files,
                  int            n_files,
                  const char    *hint)
{
  GList *windows;
  OMPAppWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = OMP_APP_WINDOW (windows->data);
  else
    win = omp_app_window_new (OMP_APP (app));

  for (i = 0; i < n_files; i++)
    omp_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

static void
omp_app_class_init (OMPAppClass *self)
{
  G_APPLICATION_CLASS (self)->activate = omp_app_activate;
  G_APPLICATION_CLASS (self)->open = omp_app_open;
}

OMPApp *
omp_app_new (void)
{
  return (OMPApp*)g_object_new (OMP_APP_TYPE,
                       "application-id", "org.gtk.omp",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}
