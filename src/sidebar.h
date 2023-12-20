#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define OMP_SIDEBAR_TYPE (omp_sidebar_get_type ())
G_DECLARE_FINAL_TYPE (OMPSidebar, omp_sidebar, OMP, APP_SIDEBAR, AdwBin)

G_END_DECLS
