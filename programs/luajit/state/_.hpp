#pragma once

#include <api.hpp>
#include <lua.hpp>

#include "mode.hpp"

#define GD%NAME%_Name "GD%NAME%"

template<VerifyMode mode>
%NAME%* state_%NAMEL%_verify( lua_State* L, int pos ){
	if constexpr ( mode == VerifyMode::Check ) {
		return (%NAME%*) luaL_checkudata( L, pos, GD%NAME%_Name );
	}
	return (%NAME%*) luaL_testudata( L, pos, GD%NAME%_Name );
}

void state_%NAMEL%_metatable( lua_State* L );
int state_%NAMEL%_push( lua_State* L, %NAME% %NAMEL% );
