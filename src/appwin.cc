#include <adwaita.h>
#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"

struct _ExampleAppWindow
{
  AdwApplicationWindow parent;
};

G_DEFINE_TYPE(ExampleAppWindow, example_app_window, ADW_TYPE_APPLICATION_WINDOW);

static void
example_app_window_init (ExampleAppWindow *win)
{
  gtk_widget_init_template (GTK_WIDGET (win));
}

static void
example_app_window_class_init (ExampleAppWindowClass *self)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (self),
                                               "/org/gtk/omp/ui/window.ui");
}

ExampleAppWindow *
example_app_window_new (ExampleApp *app)
{
  return (ExampleAppWindow*)g_object_new (EXAMPLE_APP_WINDOW_TYPE, "application", app, NULL);
}

void
example_app_window_open (ExampleAppWindow *win,
                         GFile            *file)
{
}
