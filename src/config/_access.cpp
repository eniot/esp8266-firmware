#include <Data.h>
#include "config/_access.h"
#include "config/_addr.h"

config_access_t config_access_get()
{
    config_access_t data;
    data.access = Data.readStr(_ACCESS_PASSWORD_ADDR, _ACCESS_PASSWORD_SIZE);
    return data;
}

void config_access_set(config_access_t data)
{
    Data.writeStr(_ACCESS_PASSWORD_ADDR, data.access, true);
}

void config_access_save(config_access_t data)
{
    config_access_set(data);
    Data.save();
}

config_access_t config_access_default() 
{
    config_access_t adata;
    adata.access = "";
    return adata;
}