#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define OMP_PANEL_TYPE (omp_panel_get_type ())
G_DECLARE_DERIVABLE_TYPE (OMPPanel, omp_panel, OMP, APP_PANEL, AdwBin)

struct _OMPPanelClass
{
  AdwBinClass parent_class;
};

void omp_panel_set_sidebar_entry (const gchar* panel_name);

G_END_DECLS
