#include "gtk.h"

gnuitar_error_t
gnuitar_package_add_to_clist(const gnuitar_package_t *package, GtkWidget *widget)
{
    unsigned int i;
    unsigned int count;
    const char * effect_name[2];

    effect_name[1] = NULL;

    count = gnuitar_package_get_effect_count(package);
    for (i = 0; i < count; i++){
        effect_name[0] = gnuitar_package_get_effect_name(package, i);
        if (effect_name[0] == NULL)
            continue;
        gtk_clist_append(GTK_CLIST(widget), (char **) effect_name);
    }

    return GNUITAR_ERROR_NONE;
}

