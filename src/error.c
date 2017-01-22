#include "error.h"

const char *
gnuitar_strerror(gnuitar_error_t error)
{
    switch (error) {
    case GNUITAR_ERROR_NONE:
        return "None";
    case GNUITAR_ERROR_MALLOC:
        return "Memory allocation failure";
    case GNUITAR_ERROR_ENOENT:
        return "Entry does not exist";
    case GNUITAR_ERROR_EEXISTS:
        return "Entry already exists";
    default:
        break;
    }
    return "Unknown";
}

