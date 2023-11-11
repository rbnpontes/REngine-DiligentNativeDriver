#include "DataBlob.h"

RENGINE u64 rengine_datablob_getlength(Diligent::IDataBlob* blob)
{
    return blob->GetSize();
}

RENGINE void* rengine_datablob_getdata(Diligent::IDataBlob* blob)
{
    return blob->GetDataPtr();
}
