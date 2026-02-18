#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDCallable_Name "GDCallable"

template<VerifyMode mode>
Callable* state_callable_verify( lua_State* L, int pos ) {
	if constexpr ( mode == VerifyMode::Check ) {
		return (Callable*) luaL_checkudata( L, pos, GDCallable_Name );
	}
	return (Callable*) luaL_testudata( L, pos, GDCallable_Name );
}

void state_callable_metatable( lua_State* L );
int state_callable_push( lua_State* L, Callable callable );
Callable state_callable_from_function( lua_State* L, int pos );
