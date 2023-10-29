#include <adwaita.h>
#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"

struct _OMPAppWindow
{
  AdwApplicationWindow parent;
};

G_DEFINE_TYPE(OMPAppWindow, omp_app_window, ADW_TYPE_APPLICATION_WINDOW);

static void
omp_app_window_init (OMPAppWindow *win)
{
  gtk_widget_init_template (GTK_WIDGET (win));
}

static void
omp_app_window_class_init (OMPAppWindowClass *self)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (self),
                                               "/org/gtk/omp/ui/window.ui");
}

OMPAppWindow *
omp_app_window_new (OMPApp *app)
{
  return (OMPAppWindow*) g_object_new (OMP_APP_WINDOW_TYPE, "application", app, NULL);
}

void
omp_app_window_open (OMPAppWindow *win,
                     GFile        *file)
{
}
