#include <adwaita.h>
#include <gtk/gtk.h>

#include "sidebar.h"

struct _OMPSidebar
{
  AdwBin parent;
};

G_DEFINE_TYPE(OMPSidebar, omp_sidebar, ADW_TYPE_BIN);

static void
omp_sidebar_init (OMPSidebar *sidebar)
{
  gtk_widget_init_template (GTK_WIDGET (sidebar));
}

static void
omp_sidebar_class_init (OMPSidebarClass *self)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (self),
                                               "/com/openmusicplayer/omp/ui/sidebar.ui");
}

OMPSidebar *
omp_sidebar_new (void)
{
  return (OMPSidebar*) g_object_new (OMP_SIDEBAR_TYPE, NULL);
}
