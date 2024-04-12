#include <adwaita.h>
#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"
#include "content.h"
#include "sidebar.h"

struct _OMPAppWindow {
    AdwApplicationWindow parent;

    GtkWidget* content;
    GtkWidget* sidebar;
    GtkWidget* split_view;
};

G_DEFINE_TYPE (OMPAppWindow, omp_app_window, ADW_TYPE_APPLICATION_WINDOW);

//
// Inits
//
static void
omp_app_window_init (OMPAppWindow* win)
{
    g_type_ensure (OMP_SIDEBAR_TYPE);
    g_type_ensure (OMP_CONTENT_TYPE);
    gtk_widget_init_template (GTK_WIDGET (win));
}

static void
omp_app_window_class_init (OMPAppWindowClass* self)
{
    // Set template.
    gtk_widget_class_set_template_from_resource (
        GTK_WIDGET_CLASS (self), "/com/openmusicplayer/omp/ui/window.ui"
    );

    // Set style.
    GtkCssProvider* window_css_provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_resource (
        window_css_provider, "/com/openmusicplayer/omp/ui/window.css"
    );
    gtk_style_context_add_provider_for_display (
        gdk_display_get_default (), GTK_STYLE_PROVIDER (window_css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    // Bind children from template.
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, content
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, sidebar
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, split_view
    );
}

//
// Member Functions
//
OMPAppWindow*
omp_app_window_new (OMPApp* app)
{
    return (OMPAppWindow*)g_object_new (
        OMP_APP_WINDOW_TYPE, "application", app, NULL
    );
}

void
omp_app_window_open (OMPAppWindow* win, GFile* file)
{
}

OMPContent*
omp_app_window_get_content (OMPAppWindow* win)
{
    return (OMPContent*)win->content;
}

void
omp_app_window_show_sidebar (OMPAppWindow* win)
{
    AdwOverlaySplitView* split_view = (AdwOverlaySplitView*)win->split_view;
    adw_overlay_split_view_set_show_sidebar (split_view, true);
}
