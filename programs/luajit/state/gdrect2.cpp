#include "gdrect2.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( rect2, Rect2* )

void state_rect2_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDRect2_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDRect2_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Rect2* ud = check_rect2( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				// TODO

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
			Rect2* ud = check_rect2( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				// TODO
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Rect2* ud = test_rect2( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDRect2_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDRect2_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_rect2_push( lua_State* L, Rect2 rect2 ) {
	Rect2* ud = (Rect2*) lua_newuserdata( L, sizeof( Rect2 ) );
	*ud = rect2;

	state_rect2_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
