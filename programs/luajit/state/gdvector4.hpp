#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDVector4_Name "GDVector4"

template<VerifyMode mode>
Vector4* state_vector4_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Vector4*) luaL_checkudata( L, pos, GDVector4_Name );
	}
	return (Vector4*) luaL_testudata( L, pos, GDVector4_Name );
}

void state_vector4_metatable( lua_State* L );
int state_vector4_push( lua_State* L, Vector4 vector4 );
