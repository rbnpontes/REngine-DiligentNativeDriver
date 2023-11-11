#include "PipelineStateCache.h"

RENGINE Diligent::IDataBlob* rengine_pscache_getdata(Diligent::IPipelineStateCache* cache)
{
	Diligent::IDataBlob* blob;
	cache->GetData(&blob);
	return blob;
}
