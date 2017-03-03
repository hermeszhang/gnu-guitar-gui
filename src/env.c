#include "env.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define gnuitar_stdup _strdup
#else /* _MSC_VER */
#define gnuitar_strdup strdup
#endif /* _MSC_VER */

void
gnuitar_env_init(struct GnuitarEnv *env)
{
    const char *tmp;

    env->packages_path = NULL;
    env->recordings_path = NULL;
    env->presets_path = NULL;

    tmp = getenv("GNUITAR_PACKAGES_PATH");
    if (tmp != NULL)
        env->packages_path = gnuitar_strdup(tmp);

    tmp = getenv("GNUITAR_PRESETS_PATH");
    if (tmp != NULL)
        env->presets_path = gnuitar_strdup(tmp);

    tmp = getenv("GNUITAR_RECORDINGS_PATH");
    if (tmp != NULL)
        env->recordings_path = gnuitar_strdup(tmp);
}

void
gnuitar_env_done(struct GnuitarEnv *env)
{
    free(env->packages_path);
    free(env->presets_path);
    free(env->recordings_path);
}

const char *
gnuitar_env_get(const struct GnuitarEnv *env, const char *key)
{
    if (strcmp(key, "GNUITAR_PACKAGES_PATH") == 0)
        return env->packages_path;
    else if (strcmp(key, "GNUITAR_PRESETS_PATH") == 0)
        return env->presets_path;
    else if (strcmp(key, "GNUITAR_RECORDINGS_PATH") == 0)
        return env->recordings_path;
    return NULL;
}

int
gnuitar_env_set(struct GnuitarEnv *env, const char *key, const char *value)
{
    if (strcmp(key, "GNUITAR_PACKAGES_PATH") == 0) {
        env->packages_path = gnuitar_strdup(value);
        if (env->packages_path == NULL)
            return ENOMEM;
    } else if (strcmp(key, "GNUITAR_PRESETS_PATH") == 0) {
        env->presets_path = gnuitar_strdup(value);
        if (env->presets_path == NULL)
            return ENOMEM;
    } else if (strcmp(key, "GNUITAR_RECORDINGS_PATH") == 0) {
        env->recordings_path = gnuitar_strdup(value);
        if (env->recordings_path == NULL)
            return ENOMEM;
    }
    return ENOENT;
}

