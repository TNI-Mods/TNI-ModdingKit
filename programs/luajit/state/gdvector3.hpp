#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDVector3_Name "GDVector3"

template<VerifyMode mode>
Vector3* state_vector3_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Vector3*) luaL_checkudata( L, pos, GDVector3_Name );
	}
	return (Vector3*) luaL_testudata( L, pos, GDVector3_Name );
}

void state_vector3_metatable( lua_State* L );
int state_vector3_push( lua_State* L, Vector3 vector3 );
