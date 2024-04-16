#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define OMP_TYPE_CONTEXT_PANEL (omp_context_panel_get_type ())
G_DECLARE_FINAL_TYPE (
    OMPContextPanel, omp_context_panel, OMP, CONTEXT_PANEL, AdwBin
)

G_END_DECLS
