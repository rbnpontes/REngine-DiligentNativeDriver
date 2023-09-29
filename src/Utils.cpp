#include "Api.h"

RENGINE void rengine_free(void* ptr) {
	delete ptr;
}