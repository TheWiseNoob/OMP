#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define OMP_APP_TYPE (omp_app_get_type ())
G_DECLARE_FINAL_TYPE (OMPApp, omp_app, OMP, APP, AdwApplication)

OMPApp* omp_app_new (void);

G_END_DECLS
