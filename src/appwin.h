#ifndef __OMPAPPWIN_H
#define __OMPMPLEAPPWIN_H

#include <adwaita.h>
#include <gtk/gtk.h>
#include "app.h"


#define OMP_APP_WINDOW_TYPE (omp_app_window_get_type ())
G_DECLARE_FINAL_TYPE (OMPAppWindow, omp_app_window, OMP, APP_WINDOW, AdwApplicationWindow)


OMPAppWindow *omp_app_window_new (OMPApp *app);

void omp_app_window_open (OMPAppWindow *win,
                          GFile        *file);

#endif /* __OMPAPPWIN_H */
