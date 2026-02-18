#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDColor_Name "GDColor"

template<VerifyMode mode>
Color* state_color_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Color*) luaL_checkudata( L, pos, GDColor_Name );
	}
	return (Color*) luaL_testudata( L, pos, GDColor_Name );
}

void state_color_metatable( lua_State* L );
int state_color_push( lua_State* L, Color color );
