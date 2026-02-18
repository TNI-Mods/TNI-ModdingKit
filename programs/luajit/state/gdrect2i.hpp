#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDRect2i_Name "GDRect2i"

template<VerifyMode mode>
Rect2i* state_rect2i_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Rect2i*) luaL_checkudata( L, pos, GDRect2i_Name );
	}
	return (Rect2i*) luaL_testudata( L, pos, GDRect2i_Name );
}

void state_rect2i_metatable( lua_State* L );
int state_rect2i_push( lua_State* L, Rect2i rect2i );
