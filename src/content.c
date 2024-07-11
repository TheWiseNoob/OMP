#include <adwaita.h>
#include <gtk/gtk.h>

#include "content.h"

#include "app.h"
#include "appwin.h"
#include "panels/context/context-panel.h"
#include "sidebar.h"

#include "taglib/tag_c.h"

struct _OMPContent {
    AdwBin parent;

    GtkWidget* content_label;
    GtkWidget* open_sidebar_overlay_button;
    GtkWidget* tag_label;
};

G_DEFINE_TYPE (OMPContent, omp_content, ADW_TYPE_BIN);

//
// Signals
//
enum { SHOW_SIDEBAR, LAST_SIGNAL };
static gint signals[LAST_SIGNAL] = {
    0,
};

//
// Callbacks
//
static void
omp_content_change_content (
    OMPSidebar* sidebar, const gchar* page_name, OMPContent* content
)
{
    omp_content_set_content (content, page_name);
}

static void
omp_content_open_file (GObject* gobject, GAsyncResult* result, gpointer data)
{
    g_autoptr (GError) error = NULL;
    g_autoptr (GFile) gfile = gtk_file_dialog_open_finish (
        GTK_FILE_DIALOG (gobject), result, &error
    );

    OMPContent* content = data;

    if (error != NULL) {
    }
    else {
        const char* filename = g_file_get_path (gfile);

        TagLib_File* file;
        TagLib_Tag* tag;

        file = taglib_file_new (filename);

        if (file == NULL)
            return;

        tag = taglib_file_tag (file);

        if (tag != NULL) {
            char* tag_str = "Title   - ";
            char year[5];
            sprintf (year, "%d", taglib_tag_year (tag));
            char track[5];
            sprintf (track, "%d", taglib_tag_track (tag));
            const char* final_tag_str = g_strconcat (
                tag_str, taglib_tag_title (tag), "\n", "Artist  - ",
                taglib_tag_artist (tag), "\n", "Album   - ",
                taglib_tag_album (tag), "\n", "Year    - ", year, "\n",
                "Comment - ", taglib_tag_comment (tag), "\n", "Track   - ",
                track, "\n", "Genre   - ", taglib_tag_genre (tag), "\n", NULL
            );
            gtk_label_set_text ((GtkLabel*)(content->tag_label), final_tag_str);
        }
    }
}

static void
omp_content_open_file_clicked (GtkButton* source, OMPContent* content)
{
    GtkFileDialog* dialog;
    OMPAppWindow* parent_window
        = omp_app_get_main_window ((OMPApp*)g_application_get_default ());

    dialog = gtk_file_dialog_new ();
    gtk_file_dialog_open (
        dialog, (GtkWindow*)(parent_window), NULL,
        (GAsyncReadyCallback)(omp_content_open_file), (gpointer)(content)
    );
}

static void
omp_content_open_sidebar_clicked (GtkButton* source, OMPContent* content)
{
    g_signal_emit (content, signals[SHOW_SIDEBAR], 0);

    g_object_set (source, "visible", false, NULL);
}

static void
omp_content_show_open_sidebar_overlay_button (
    OMPSidebar* sidebar, const gboolean show, OMPContent* content
)
{
    g_object_set (content->open_sidebar_overlay_button, "visible", show, NULL);
}

static void
omp_content_show_sidebar (OMPSidebar* sidebar, OMPContent* content)
{
    OMPAppWindow* main_window
        = omp_app_get_main_window ((OMPApp*)g_application_get_default ());
    omp_app_window_show_sidebar (main_window);
}

//
// Inits
//
static void
omp_content_init (OMPContent* content)
{
    g_type_ensure (OMP_CONTEXT_PANEL_TYPE);

    gtk_widget_init_template (GTK_WIDGET (content));

    omp_app_set_content (OMP_APP (g_application_get_default ()), content);

    // Callbacks
    OMPApp* app = OMP_APP (g_application_get_default ());
    OMPSidebar* sidebar = omp_app_get_sidebar (app);
    g_signal_connect (
        sidebar, "change-content", G_CALLBACK (omp_content_change_content),
        content
    );
    g_signal_connect (
        sidebar, "show-open-sidebar-overlay-button",
        G_CALLBACK (omp_content_show_open_sidebar_overlay_button), content
    );
    g_signal_connect (
        content, "show-sidebar", G_CALLBACK (omp_content_show_sidebar), sidebar
    );

    // Style overl sidebar show button.
    gtk_widget_add_css_class (
        content->open_sidebar_overlay_button, "open_sidebar_overlay_button"
    );

    // Set what the content will be.
    omp_content_set_content (content, "Context");
}

static void
omp_content_class_init (OMPContentClass* self)
{
    // Set template.
    gtk_widget_class_set_template_from_resource (
        GTK_WIDGET_CLASS (self), "/com/openmusicplayer/omp/ui/content.ui"
    );

    // Set style.
    GtkCssProvider* window_css_provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_resource (
        window_css_provider, "/com/openmusicplayer/omp/ui/content.css"
    );
    gtk_style_context_add_provider_for_display (
        gdk_display_get_default (), GTK_STYLE_PROVIDER (window_css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    // Bind children from template.
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPContent, content_label
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPContent, open_sidebar_overlay_button
    );
    gtk_widget_class_bind_template_child (
        GTK_WIDGET_CLASS (self), OMPContent, tag_label
    );

    // Callbacks
    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), omp_content_open_file_clicked
    );
    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), omp_content_open_sidebar_clicked
    );
    gtk_widget_class_bind_template_callback (
        GTK_WIDGET_CLASS (self), omp_content_show_open_sidebar_overlay_button
    );

    // Signals
    signals[SHOW_SIDEBAR] = g_signal_new (
        "show-sidebar", OMP_CONTENT_TYPE, G_SIGNAL_RUN_LAST, 0, NULL, NULL,
        NULL, G_TYPE_NONE, 0
    );
}

void
omp_content_set_content (OMPContent* content, const gchar* page_name)
{
    gtk_label_set_text ((GtkLabel*)(content->content_label), page_name);
}
