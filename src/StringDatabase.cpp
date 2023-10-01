#include "StringDatabase.h"
#include <vector>
#include <string>

static std::vector<std::string> g_strings;
static uint g_next = 0;


const char* rengine_stringdb_store(const char* str)
{
	if (g_next >= g_strings.size()) {
		g_strings.push_back(str);
	}
	else {
		g_strings[g_next] = str;
	}

	str = g_strings[g_next].c_str();
	++g_next;
	return str;
}

void rengine_stringdb_reset()
{
	g_next = 0;
}

RENGINE void rengine_stringdb_free()
{
	g_strings.clear();
}
