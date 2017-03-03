#ifndef GNUITAR_ENV_H
#define GNUITAR_ENV_H

struct GnuitarEnv {
    char *packages_path;
    char *presets_path;
    char *recordings_path;
};

void gnuitar_env_init(struct GnuitarEnv *env);

void gnuitar_env_done(struct GnuitarEnv *env);

const char * gnuitar_env_get(const struct GnuitarEnv *env, const char *key);

int gnuitar_env_set(struct GnuitarEnv *env, const char *key, const char *value);

#endif /* GNUITAR_ENV_H */

