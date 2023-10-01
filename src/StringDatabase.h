#pragma once
#include "Api.h"

// String Database is used to Store Temporarly
// Strings that will be sended to .NET
// The implementation consists in a internal pool
// of strings that we store along of execution
// this prevents string to be destroyed


/// @{
/// Store string and returns stored string ptr
/// @}
const char* rengine_stringdb_store(const char* str);
/// @{
/// Resets internal counting
/// This method must be called at every time your feature is called
/// @}
void rengine_stringdb_reset();
/// @{
/// Free allocated internal strings
/// This method must be called by .NET
/// @}
RENGINE void rengine_stringdb_free();
