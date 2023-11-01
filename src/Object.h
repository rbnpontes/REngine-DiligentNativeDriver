#pragma once

#include "Api.h"
#include <DeviceObject.h>

RENGINE void rengine_object_releaseref(Diligent::IObject* object);
RENGINE void rengine_object_addref(Diligent::IObject* object);
RENGINE void rengine_object_set_release_callback(Diligent::IObject* object, Diligent::ObjectReleaseCallbackType releaseCallback);
RENGINE u32 rengine_object_strongref_count(Diligent::IObject* object);
RENGINE u32 rengine_object_weakref_count(Diligent::IObject* object);
RENGINE string* rengine_object_getname(Diligent::IDeviceObject* object);