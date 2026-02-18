#pragma once

#include <lua.hpp>

int debug_stacktrace_handler( lua_State *L );
int pcall_stacktrace( lua_State* L, int nargs, int nret );
