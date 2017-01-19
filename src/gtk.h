#ifndef GNUITAR_GTK_H
#define GNUITAR_GTK_H

#include <gtk/gtk.h>

#include "error.h"
#include "package.h"

#if GTK_CHECK_VERSION(3, 0, 0)

#else /* GTK_CHECK_VERSION(3, 0, 0) */

gnuitar_error_t gnuitar_package_add_to_clist(const gnuitar_package_t *package, GtkWidget *clist);

#endif /* GTK_CHECK_VERSION(3, 0, 0) */

#endif /* GNUITAR_GTK_H */

