#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GDTransform3D_Name "GDTransform3D"

template<VerifyMode mode>
Transform3D* state_transform3d_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (Transform3D*) luaL_checkudata( L, pos, GDTransform3D_Name );
	}
	return (Transform3D*) luaL_testudata( L, pos, GDTransform3D_Name );
}

void state_transform3d_metatable( lua_State* L );
int state_transform3d_push( lua_State* L, Transform3D transform3d );
