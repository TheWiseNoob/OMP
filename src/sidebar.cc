#include "sidebar.h"

#include <adwaita.h>
#include <gtk/gtk.h>

struct _OMPSidebar
{
  AdwBin parent;

  GtkWidget *show_sidebar_button;
  gboolean sidebar_button_active;
};

enum
{
    PROP_0,
    PROP_SIDEBAR_BUTTON_ACTIVE,
    NUM_PROPERTIES
};

static GParamSpec *properties[NUM_PROPERTIES] = { NULL, };

G_DEFINE_TYPE(OMPSidebar, omp_sidebar, ADW_TYPE_BIN);

static void
output_state (GtkToggleButton *source,
              gpointer         user_data)
{
  g_print ("Toggle button derp derp");
}

static void
omp_sidebar_get_property (GObject    *object,
                          guint       property_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
    OMPSidebar *self = OMP_APP_SIDEBAR (object);

    switch (property_id)
    {
        case PROP_SIDEBAR_BUTTON_ACTIVE:
        {
            g_value_set_boolean (value, self->sidebar_button_active);
        }
        break;

        default:
        {
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        }
        break;
    }
}

static void
omp_sidebar_set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
    OMPSidebar *self = OMP_APP_SIDEBAR (object);

    switch (property_id)
    {
        case PROP_SIDEBAR_BUTTON_ACTIVE:
        {
            self->sidebar_button_active = g_value_get_boolean (value);
        }
        break;

        default:
        {
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        }
        break;
    }
}

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

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (self), OMPSidebar, show_sidebar_button);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (self), output_state);


  GObjectClass *oclass;
  GtkWidgetClass *widget_class;

  widget_class = GTK_WIDGET_CLASS (self);
  oclass = G_OBJECT_CLASS (self);
  oclass->get_property = omp_sidebar_get_property;
  oclass->set_property = omp_sidebar_set_property;

  properties[PROP_SIDEBAR_BUTTON_ACTIVE] =
      g_param_spec_boolean ("sidebar-button-active", NULL, NULL, FALSE,
                            (GParamFlags)(G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (oclass, NUM_PROPERTIES, properties);
}

OMPSidebar *
omp_sidebar_new (void)
{
  return (OMPSidebar*) g_object_new (OMP_SIDEBAR_TYPE, NULL);
}
