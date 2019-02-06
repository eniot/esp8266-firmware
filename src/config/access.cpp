#include <Data.h>
#include "config/access.h"
#include "config/_addr.h"

config_access_t *_access_cache = NULL;

config_access_t config_access_get()
{
    config_access_t data;
    config_access_get(&data);
    return data;
}

void config_access_get(config_access_t *data)
{
    if (_access_cache != NULL)
        data->access = _access_cache->access;
    else
        data->access = Data.readStr(_ACCESS_PASSWORD_ADDR, _ACCESS_PASSWORD_SIZE);
}

void config_access_set(config_access_t data)
{
    Data.writeStr(_ACCESS_PASSWORD_ADDR, data.access);
}

void config_access_save(config_access_t data)
{
    config_access_set(data);
    Data.save();
    _access_cache = NULL;
}

config_access_t config_access_default()
{
    config_access_t adata;
    adata.access = "";
    return adata;
}