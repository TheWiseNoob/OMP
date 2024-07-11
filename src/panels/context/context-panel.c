#include <adwaita.h>

#include "context-panel.h"

struct _OMPContextPanel {
    OMPPanel parent;
};

G_DEFINE_TYPE (OMPContextPanel, omp_context_panel, OMP_PANEL_TYPE);

static void
omp_context_panel_class_init (OMPContextPanelClass* klass)
{
    GtkWidgetClass* widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource (
        widget_class,
        "/com/openmusicplayer/omp/ui/context.ui"
    );
}

static void
omp_context_panel_init (OMPContextPanel* self)
{
    gtk_widget_init_template (GTK_WIDGET (self));

    omp_panel_set_sidebar_entry("Herpy");
}
