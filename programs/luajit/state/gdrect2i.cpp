#include "gdrect2i.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( rect2i, Rect2i* )

void state_rect2i_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDRect2i_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDRect2i_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Rect2i* ud = check_rect2i( L, 1 );
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
			Rect2i* ud = check_rect2i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				// TODO
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Rect2i* ud = test_rect2i( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDRect2i_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDRect2i_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_rect2i_push( lua_State* L, Rect2i rect2i ) {
	Rect2i* ud = (Rect2i*) lua_newuserdata( L, sizeof( Rect2i ) );
	*ud = rect2i;

	state_rect2i_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
