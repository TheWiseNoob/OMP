#include "panel.h"

typedef struct _OMPPanelPrivate OMPPanelPrivate;
struct _OMPPanelPrivate {
    AdwBin parent;
};

G_DEFINE_TYPE_WITH_PRIVATE (OMPPanel, omp_panel, ADW_TYPE_BIN);

//
// Inits
//
static void
omp_panel_init (OMPPanel* sidebar)
{
}

static void
omp_panel_class_init (OMPPanelClass* self)
{
}

void
omp_panel_set_sidebar_entry (const gchar* panel_name)
{
    g_print("panel name: %s", panel_name);
}
