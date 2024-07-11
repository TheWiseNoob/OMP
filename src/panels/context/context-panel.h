#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>
#include "../../panel.h"

G_BEGIN_DECLS

#define OMP_CONTEXT_PANEL_TYPE (omp_context_panel_get_type ())
G_DECLARE_FINAL_TYPE (
    OMPContextPanel, omp_context_panel, OMP, APP_CONTEXT_PANEL, OMPPanel
)

G_END_DECLS
