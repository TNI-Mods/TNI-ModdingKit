#include "gd%NAMEL%.hpp"
#include "tower.hpp"
#include "state.hpp"

VERIFY_MODE_WRAPPER( %NAMEL%, %NAME%* )

void state_%NAMEL%_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GD%NAME%_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GD%NAME%_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			%NAME%* ud = check_%NAMEL%( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );


				if ( ! ( (Mod) get_node() ).is_allowed_property( *ud, name ) ) {
					return luaL_error( L, "Banned property accessed: %s", name.c_str() );  // This does not return
				}
			}
			lua_pushnil( L );
			return 1;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__newindex" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			%NAME%* ud = check_%NAMEL%( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			%NAME%* ud = test_%NAMEL%( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GD%NAME%_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GD%NAME%_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_%NAMEL%_push( lua_State* L, %NAME% %NAMEL% ) {
	%NAME%* ud = (%NAME%*) lua_newuserdata( L, sizeof( %NAME% ) );
	*ud = %NAMEL%;

	state_%NAMEL%_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
