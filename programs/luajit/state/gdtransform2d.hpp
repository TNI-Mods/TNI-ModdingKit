#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDTransform2D_Name "GDTransform2D"

template<VerifyMode mode>
Transform2D* state_transform2d_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Transform2D*) luaL_checkudata( L, pos, GDTransform2D_Name );
	}
	return (Transform2D*) luaL_testudata( L, pos, GDTransform2D_Name );
}

void state_transform2d_metatable( lua_State* L );
int state_transform2d_push( lua_State* L, Transform2D transform2d );
