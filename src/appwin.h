#pragma once

#include "app.h"
#include <adwaita.h>
#include <gtk/gtk.h>

typedef struct _OMPContent OMPContent;

G_BEGIN_DECLS

#define OMP_APP_WINDOW_TYPE (omp_app_window_get_type ())
G_DECLARE_FINAL_TYPE (
    OMPAppWindow, omp_app_window, OMP, APP_WINDOW, AdwApplicationWindow
)

OMPAppWindow* omp_app_window_new (OMPApp* app);

void omp_app_window_open (OMPAppWindow* win, GFile* file);

OMPContent* omp_app_window_get_content (OMPAppWindow* win);

void omp_app_window_show_sidebar (OMPAppWindow* win);

G_END_DECLS
