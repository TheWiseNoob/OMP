#ifndef __OMPAPP_H
#define __OMPAPP_H

#include <adwaita.h>
#include <gtk/gtk.h>


#define OMP_APP_TYPE (omp_app_get_type ())
G_DECLARE_FINAL_TYPE (OMPApp, omp_app, OMP, APP, AdwApplication)


OMPApp *omp_app_new (void);


#endif /* __OMPAPP_H */
