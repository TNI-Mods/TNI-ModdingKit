#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDRect2_Name "GDRect2"

template<VerifyMode mode>
Rect2* state_rect2_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Rect2*) luaL_checkudata( L, pos, GDRect2_Name );
	}
	return (Rect2*) luaL_testudata( L, pos, GDRect2_Name );
}

void state_rect2_metatable( lua_State* L );
int state_rect2_push( lua_State* L, Rect2 rect2 );
