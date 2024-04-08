#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

typedef struct _OMPAppWindow OMPAppWindow;
typedef struct _OMPContent OMPContent;
typedef struct _OMPSidebar OMPSidebar;

G_BEGIN_DECLS

#define OMP_APP_TYPE (omp_app_get_type ())
G_DECLARE_FINAL_TYPE (OMPApp, omp_app, OMP, APP, AdwApplication)

OMPApp* omp_app_new (void);

OMPAppWindow* omp_app_get_main_window (OMPApp* app);

OMPContent* omp_app_get_content (OMPApp* app);
void omp_app_set_content (OMPApp* app, OMPContent* content);

OMPSidebar* omp_app_get_sidebar (OMPApp* app);
void omp_app_set_sidebar (OMPApp* app, OMPSidebar* sidebar);

G_END_DECLS
