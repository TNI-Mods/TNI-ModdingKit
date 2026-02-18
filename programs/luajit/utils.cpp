#include "utils.hpp"

// From: https://github.com/sydlawrence/CorsixTH-HTML5-Port/blob/95d272e0ad27758fed7d231d8ab7f79aa4b0773f/source/CorsixTH/Src/main.cpp#L201
int debug_stacktrace_handler( lua_State *L ) {
	// ..., err_obj
	lua_insert( L, 1 );
	// err_obj, ...
	lua_settop( L, 1 );
	// err_obj

	lua_getglobal( L, "tostring" );
	lua_insert( L, 1 );
	lua_call( L, 1, 1 );
	// err_str

	lua_getglobal( L, "debug" );
	lua_getfield( L, -1, "traceback" );
	// err_str, debug, traceback
	lua_remove( L, -2 );
	// err_str, traceback
	lua_insert( L, 1 );
	// traceback, err_str
	lua_pushinteger( L, 2 );
	// traceback, err_str, 2
	lua_call( L, 2, 1 );
	// stacktrace_str

	return 1;
}

int pcall_stacktrace( lua_State* L, int nargs, int nret ) {
	int hpos = lua_gettop( L ) - nargs;
	lua_pushcfunction( L, debug_stacktrace_handler );
	lua_insert( L, hpos );
	int ret = lua_pcall( L, nargs, nret, hpos );
	lua_remove( L, hpos );
	return ret;
}
