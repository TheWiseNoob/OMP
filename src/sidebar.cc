#include "sidebar.h"

#include "appwin.h"
#include <adwaita.h>
#include <gtk/gtk.h>

struct _OMPSidebar {
    AdwBin parent;

    // Widgets
    GtkWidget* toggle_sidebar_button;
    gboolean sidebar_button_active;
};
G_DEFINE_TYPE (OMPSidebar, omp_sidebar, ADW_TYPE_BIN);

// Properties
enum { PROP_0, PROP_SIDEBAR_BUTTON_ACTIVE, NUM_PROPERTIES };
static GParamSpec* properties[NUM_PROPERTIES] = {
    NULL,
};

// Signals
enum { SHOW_SIDEBAR, LAST_SIGNAL };
static gint signals[LAST_SIGNAL] = {
    0,
};

static void
toggle_button_clicked (GtkToggleButton* source, OMPSidebar* sidebar)
{
    g_signal_emit (sidebar, signals[SHOW_SIDEBAR], 0);
}

// Getter
static void
omp_sidebar_get_property (
    GObject* object, guint property_id, GValue* value, GParamSpec* pspec
)
{
    OMPSidebar* self = OMP_APP_SIDEBAR (object);

    switch (property_id) {
        case PROP_SIDEBAR_BUTTON_ACTIVE: {
            g_value_set_boolean (value, self->sidebar_button_active);
        } break;

        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        } break;
    }
}

// Setter
static void
omp_sidebar_set_property (
    GObject* object, guint property_id, const GValue* value, GParamSpec* pspec
)
{
    OMPSidebar* self = OMP_APP_SIDEBAR (object);

    switch (property_id) {
        case PROP_SIDEBAR_BUTTON_ACTIVE: {
            self->sidebar_button_active = g_value_get_boolean (value);
        } break;

        default: {
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        } break;
    }
}

//
// Inits
//
static void
omp_sidebar_init (OMPSidebar* sidebar)
{
    gtk_widget_init_template (GTK_WIDGET (sidebar));
}

static void
omp_sidebar_class_init (OMPSidebarClass* self)
{
    gtk_widget_class_set_template_from_resource (
        GTK_WIDGET_CLASS (self), "/com/openmusicplayer/omp/ui/sidebar.ui"
    );

    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPSidebar, toggle_sidebar_button
    );

    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), toggle_button_clicked
    );

    GObjectClass* oclass;
    oclass = G_OBJECT_CLASS (self);
    oclass->get_property = omp_sidebar_get_property;
    oclass->set_property = omp_sidebar_set_property;

    signals[SHOW_SIDEBAR] = g_signal_new (
        "show-open-sidebar-overlay-button", OMP_SIDEBAR_TYPE, G_SIGNAL_RUN_LAST,
        0, NULL, NULL, NULL, G_TYPE_NONE, 0
    );

    properties[PROP_SIDEBAR_BUTTON_ACTIVE] = g_param_spec_boolean (
        "sidebar-button-active", NULL, NULL, FALSE,
        (GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)
    );

    g_object_class_install_properties (oclass, NUM_PROPERTIES, properties);
}
