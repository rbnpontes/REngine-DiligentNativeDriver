#pragma once

#include "Api.h"
#include <DeviceObject.h>

RENGINE void rengine_object_releaseref(Diligent::IObject* object);
RENGINE void rengine_object_set_release_callback(Diligent::IObject* object, Diligent::ObjectReleaseCallbackType releaseCallback);