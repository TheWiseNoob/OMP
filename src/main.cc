#include <adwaita.h>
#include <gtk/gtk.h>


static void print_hello ()
{
  g_print ("Hello World\n");
}

static void
activate (GtkApplication *app)
{
  GtkWidget *window;
  GtkWidget *button;

  window = adw_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  GtkWidget *overlay = adw_overlay_split_view_new ();
  adw_application_window_set_content ((AdwApplicationWindow*)(window), overlay);
  adw_overlay_split_view_set_sidebar((AdwOverlaySplitView*)(overlay), button);

  gtk_window_present (GTK_WINDOW (window));
  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk_application_prefer_dark_theme", TRUE, NULL);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
