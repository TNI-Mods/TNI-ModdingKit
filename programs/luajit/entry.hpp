#pragma once

#include <api.hpp>

#include "state.hpp"
#include "utils.hpp"

static lua_State* lstate;

#define DEFINE_LUA_CALLBACK_0( name )\
	static Variant name() { \
		lua_settop( lstate, 0 ); \
		lua_getglobal( lstate, #name ); \
		if ( ! lua_isfunction( lstate, -1 ) ) { \
			lua_pop( lstate, 1 ); \
			return Nil; \
		} \
		if ( pcall_stacktrace( lstate, 0, 0 ) != 0 ) { \
			const char *err = lua_tostring( lstate, -1 ); \
			printf( "Lua error: %s\n", err ); \
			lua_pop( lstate, 1 ); \
		} \
		return Nil; \
	}

#define DEFINE_LUA_CALLBACK_1( name, type1, param1 ) \
	static Variant name( type1 param1 ) { \
		lua_settop( lstate, 0 ); \
		lua_getglobal( lstate, #name ); \
		if ( ! lua_isfunction( lstate, -1 ) ) { \
			lua_pop( lstate, 1 ); \
			return Nil; \
		} \
		int nargs = state_sol_lua_push_gd( lstate, param1 ); \
		if ( pcall_stacktrace( lstate, nargs, 0 ) != 0 ) { \
			const char *err = lua_tostring( lstate, -1 ); \
			printf( "Lua error: %s\n", err ); \
			lua_pop( lstate, 1 ); \
		} \
		return Nil; \
	}
