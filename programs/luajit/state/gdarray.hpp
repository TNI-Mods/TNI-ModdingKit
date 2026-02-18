#pragma once

#include <lua.hpp>
#include <api.hpp>

#include "mode.hpp"

#define GDArray_Name "GDArray"

template<VerifyMode mode>
Array* state_array_verify( lua_State* L, int pos ) {
	if constexpr ( mode == VerifyMode::Check ) {
		return (Array*) luaL_checkudata( L, pos, GDArray_Name );
	}
	return (Array*) luaL_testudata( L, pos, GDArray_Name );
}

void state_array_metatable( lua_State* L );
int state_array_variant_self_call( lua_State* L );
int state_array_push( lua_State* L, Array array );
