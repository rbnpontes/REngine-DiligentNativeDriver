#include "./Object.h"

RENGINE void rengine_object_releaseref(Diligent::IObject* object) {
	if (object != null)
		object->Release();
}
RENGINE void rengine_object_set_release_callback(Diligent::IObject* object, Diligent::ObjectReleaseCallbackType releaseCallback) {
	if (object != null)
		object->GetReferenceCounters()->SetReleaseCallback(releaseCallback);
}