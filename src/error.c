#include "error.h"

const char *
gnuitar_strerror(gnuitar_error_t error)
{
    switch (error) {
    case GNUITAR_ERROR_NONE:
        return "None";
    case GNUITAR_ERROR_MALLOC:
        return "Memory allocation failure";
    default:
        break;
    }
    return "Unknown";
}

