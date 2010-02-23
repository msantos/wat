
#include <string.h>
#include <stdio.h>

#include "erl_nif.h"

#define VAL(data, index)    data->data[index].val
#define NELEM(data)         data->nelem

typedef struct {
    int val;
} PRIVVAL;

typedef struct {
    int nelem;
    PRIVVAL *data;
} PRIVDATA;

static ERL_NIF_TERM error_tuple(ErlNifEnv *env, char *err);


    static int
load(ErlNifEnv *env, void **priv, ERL_NIF_TERM load_info)
{
    PRIVDATA *data = NULL;
    int nelem = 0;


    if (!enif_get_int(env, load_info, &nelem) || (nelem < 1))
        return (-1);

    data = (PRIVDATA *)enif_alloc(env, sizeof(PRIVDATA));

    if (data == NULL)
        return (-1);

    data->data = (PRIVVAL *)enif_alloc(env, sizeof(PRIVVAL) * nelem);
    if (data->data == NULL) {
        enif_free(env, data);
        return (-1);
    }

    (void)memset(data->data, 0, sizeof(PRIVVAL) * nelem);

    NELEM(data) = nelem;
    *priv = data;

    return (0);
}


    static int
reload(ErlNifEnv *env, void **priv, ERL_NIF_TERM load_info)
{
    enif_free(env, ((PRIVDATA *)*priv)->data);
    enif_free(env, *priv);

    return load(env, priv, load_info);
}


    static ERL_NIF_TERM
get(ErlNifEnv *env, ERL_NIF_TERM key)
{
    PRIVDATA *data = NULL;
    int k = 0;
    int nelem = 0;

    data = (PRIVDATA *)enif_get_data(env);
    nelem = NELEM(data);

    if (!enif_get_int(env, key, &k))
        return enif_make_badarg(env);

    if ( (k < 0) || (k >= nelem))
        return error_tuple(env, "out_of_bounds");

    return enif_make_int(env, VAL(data, k));
}


    static ERL_NIF_TERM
set(ErlNifEnv *env, ERL_NIF_TERM key, ERL_NIF_TERM val)
{
    PRIVDATA *data = NULL;
    int k = 0;
    int v = 0;
    int nelem = 0;

    data = (PRIVDATA *)enif_get_data(env);
    nelem = NELEM(data);

    if ( !enif_get_int(env, key, &k) ||
        !enif_get_int(env, val, &v))
        return enif_make_badarg(env);

    if ( (k < 0) || (k >= nelem))
        return error_tuple(env, "out_of_bounds");

    VAL(data, k) = v;

    return enif_make_int(env, v);
}


    static ERL_NIF_TERM
add(ErlNifEnv *env, ERL_NIF_TERM key, ERL_NIF_TERM val)
{
    PRIVDATA *data = NULL;
    int k = 0;
    int v = 0;
    int nelem = 0;

    data = (PRIVDATA *)enif_get_data(env);
    nelem = NELEM(data);

    if ( !enif_get_int(env, key, &k) ||
        !enif_get_int(env, val, &v))
        return enif_make_badarg(env);

    if ( (k < 0) || (k >= nelem))
        return error_tuple(env, "out_of_bounds");

    VAL(data, k) += v;

    return enif_make_int(env, VAL(data, k));
}


    static ERL_NIF_TERM
error_tuple(ErlNifEnv *env, char *err)
{
    return enif_make_tuple(env, 2,
            enif_make_atom(env, "error"),
            enif_make_atom(env, err));
}


static ErlNifFunc nif_funcs[] = {
    {"get", 1, get},
    {"set", 2, set},
    {"add", 2, add}
};

ERL_NIF_INIT(wat, nif_funcs, load, reload, NULL, NULL)


