#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDVector2i_Name "GDVector2i"

template<VerifyMode mode>
Vector2i* state_vector2i_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Vector2i*) luaL_checkudata( L, pos, GDVector2i_Name );
	}
	return (Vector2i*) luaL_testudata( L, pos, GDVector2i_Name );
}

void state_vector2i_metatable( lua_State* L );
int state_vector2i_push( lua_State* L, Vector2i vector2i );
