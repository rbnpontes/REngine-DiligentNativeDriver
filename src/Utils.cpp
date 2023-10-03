#include "Api.h"

RENGINE void rengine_free(void* ptr) {
	delete ptr;
}
RENGINE void rengine_free_block(void* ptr) {
	delete[] ptr;
}