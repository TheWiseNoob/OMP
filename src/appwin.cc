#include <adwaita.h>
#include <gtk/gtk.h>

#include "app.h"
#include "appwin.h"
#include "sidebar.h"

struct _OMPAppWindow {
    AdwApplicationWindow parent;

    GtkWidget* sidebar;
    GtkWidget* split_view;
    GtkWidget* open_sidebar_overlay_button;
    GtkWidget* label1;
};

G_DEFINE_TYPE (OMPAppWindow, omp_app_window, ADW_TYPE_APPLICATION_WINDOW);

static void
open_sidebar_clicked (GtkButton* source, OMPAppWindow* omp_app_window)
{
    g_object_set (omp_app_window->split_view, "show-sidebar", true, NULL);
    g_object_set (source, "visible", false, NULL);
}

static void
omp_app_window_show_open_sidebar_overlay_button (OMPAppWindow* win)
{
    g_object_set (win->open_sidebar_overlay_button, "visible", true, NULL);
}

static void
omp_app_window_change_content (OMPAppWindow* win, const gchar* page_name)
{
    gtk_label_set_text ((GtkLabel*)(win->label1), page_name);
}

static void
omp_app_window_init (OMPAppWindow* win)
{
    g_type_ensure (OMP_SIDEBAR_TYPE);
    gtk_widget_init_template (GTK_WIDGET (win));

    gtk_widget_add_css_class (
        win->open_sidebar_overlay_button, "open_sidebar_overlay_button"
    );
}

static void
omp_app_window_class_init (OMPAppWindowClass* self)
{
    // Set template.
    gtk_widget_class_set_template_from_resource (
        GTK_WIDGET_CLASS (self), "/com/openmusicplayer/omp/ui/window.ui"
    );

    // Bind children from template.
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, sidebar
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, split_view
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, open_sidebar_overlay_button
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPAppWindow, label1
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

    // Callbacks
    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), open_sidebar_clicked
    );
    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), omp_app_window_show_open_sidebar_overlay_button
    );
    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), omp_app_window_change_content
    );
}

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
