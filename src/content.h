#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define OMP_CONTENT_TYPE (omp_content_get_type ())
G_DECLARE_FINAL_TYPE (OMPContent, omp_content, OMP, APP_CONTENT, AdwBin)

void omp_content_set_content (OMPContent* content, const gchar* page_name);

G_END_DECLS
