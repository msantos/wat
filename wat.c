
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

    data = (PRIVDATA *)enif_alloc(sizeof(PRIVDATA));

    if (data == NULL)
        return (-1);

    data->data = (PRIVVAL *)enif_alloc(sizeof(PRIVVAL) * nelem);
    if (data->data == NULL) {
        enif_free(data);
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
    enif_free(((PRIVDATA *)*priv)->data);
    enif_free(*priv);

    return load(env, priv, load_info);
}


    static ERL_NIF_TERM
get(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    PRIVDATA *data = NULL;
    int k = 0;
    int nelem = 0;

    data = (PRIVDATA *)enif_priv_data(env);
    nelem = NELEM(data);

    if (!enif_get_int(env, argv[0], &k))
        return enif_make_badarg(env);

    if ( (k < 0) || (k >= nelem))
        return error_tuple(env, "out_of_bounds");

    return enif_make_int(env, VAL(data, k));
}


/* args: 0:key, 1:value */
    static ERL_NIF_TERM
set(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    PRIVDATA *data = NULL;
    int k = 0;
    int v = 0;
    int nelem = 0;

    data = (PRIVDATA *)enif_priv_data(env);
    nelem = NELEM(data);

    if ( !enif_get_int(env, argv[0], &k) ||
        !enif_get_int(env, argv[1], &v))
        return enif_make_badarg(env);

    if ( (k < 0) || (k >= nelem))
        return error_tuple(env, "out_of_bounds");

    VAL(data, k) = v;

    return enif_make_int(env, v);
}


    static ERL_NIF_TERM
add(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    PRIVDATA *data = NULL;
    int k = 0;
    int v = 0;
    int nelem = 0;

    data = (PRIVDATA *)enif_priv_data(env);
    nelem = NELEM(data);

    if ( !enif_get_int(env, argv[0], &k) ||
        !enif_get_int(env, argv[1], &v))
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


