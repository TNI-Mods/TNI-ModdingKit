#pragma once

#include <api.hpp>
#include <lua.hpp>
#include "tower.hpp"

#include "mode.hpp"

#define GDVector2_Name "GDVector2"

template<VerifyMode mode>
Vector2* state_vector2_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Vector2*) luaL_checkudata( L, pos, GDVector2_Name );
	}
	return (Vector2*) luaL_testudata( L, pos, GDVector2_Name );
}

void state_vector2_metatable( lua_State* L );
int state_vector2_push( lua_State* L, Vector2 vector );
