#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDVector3i_Name "GDVector3i"

template<VerifyMode mode>
Vector3i* state_vector3i_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Vector3i*) luaL_checkudata( L, pos, GDVector3i_Name );
	}
	return (Vector3i*) luaL_testudata( L, pos, GDVector3i_Name );
}

void state_vector3i_metatable( lua_State* L );
int state_vector3i_push( lua_State* L, Vector3i vector3i );
