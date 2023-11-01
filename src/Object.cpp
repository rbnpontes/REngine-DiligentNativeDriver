#include "./Object.h"

RENGINE void rengine_object_releaseref(Diligent::IObject* object) {
	if (object != null)
		object->Release();
}
RENGINE void rengine_object_addref(Diligent::IObject* object) {
	if (object != null)
		object->AddRef();
}
RENGINE void rengine_object_set_release_callback(Diligent::IObject* object, Diligent::ObjectReleaseCallbackType releaseCallback) {
	if (object != null)
		object->GetReferenceCounters()->SetReleaseCallback(releaseCallback);
}

RENGINE u32 rengine_object_strongref_count(Diligent::IObject* object) {
	u32 result = 0;
	if (object != null)
		result = object->GetReferenceCounters()->GetNumStrongRefs();
	return result;
}

RENGINE u32 rengine_object_weakref_count(Diligent::IObject* object) {
	u32 result = 0;
	if (object != null)
		result = object->GetReferenceCounters()->GetNumWeakRefs();
	return result;
}

RENGINE string* rengine_object_getname(Diligent::IDeviceObject* object)
{
	if (object != null)
		return object->GetDesc().Name;
	return null;
}
