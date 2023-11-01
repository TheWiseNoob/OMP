#include <adwaita.h>
#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"
#include "sidebar.h"

struct _OMPAppWindow
{
  AdwApplicationWindow parent;

  GtkWidget *sidebar;
};

G_DEFINE_TYPE(OMPAppWindow, omp_app_window, ADW_TYPE_APPLICATION_WINDOW);

static void
omp_app_window_init (OMPAppWindow *win)
{
  g_type_ensure (OMP_SIDEBAR_TYPE);
  gtk_widget_init_template (GTK_WIDGET (win));
}

static void
omp_app_window_class_init (OMPAppWindowClass *self)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (self),
                                               "/com/openmusicplayer/omp/ui/window.ui");
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (self), OMPAppWindow, sidebar);
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
