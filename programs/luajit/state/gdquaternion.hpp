#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDQuaternion_Name "GDQuaternion"

template<VerifyMode mode>
Quaternion* state_quaternion_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Quaternion*) luaL_checkudata( L, pos, GDQuaternion_Name );
	}
	return (Quaternion*) luaL_testudata( L, pos, GDQuaternion_Name );
}

void state_quaternion_metatable( lua_State* L );
int state_quaternion_push( lua_State* L, Quaternion quaternion );
