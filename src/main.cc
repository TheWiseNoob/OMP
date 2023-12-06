#include "app.h"
#include <adwaita.h>

int
main (int argc, char* argv[])
{
    return g_application_run (G_APPLICATION (omp_app_new ()), argc, argv);
}
