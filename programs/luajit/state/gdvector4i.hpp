#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDVector4i_Name "GDVector4i"

template<VerifyMode mode>
Vector4i* state_vector4i_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Vector4i*) luaL_checkudata( L, pos, GDVector4i_Name );
	}
	return (Vector4i*) luaL_testudata( L, pos, GDVector4i_Name );
}

void state_vector4i_metatable( lua_State* L );
int state_vector4i_push( lua_State* L, Vector4i vector4i );
