#include "gdcolor.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( color, Color* )

void state_color_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDColor_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDColor_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Color* ud = check_color( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				if ( name == "r" ) { lua_pushnumber( L, ud->r ); return 1; }
				if ( name == "g" ) { lua_pushnumber( L, ud->g ); return 1; }
				if ( name == "b" ) { lua_pushnumber( L, ud->b ); return 1; }
				if ( name == "a" ) { lua_pushnumber( L, ud->a ); return 1; }

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
			Color* ud = check_color( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				if      ( name == "r" ) { ud->r = lua_tonumber( L, 3 ); }
				else if ( name == "g" ) { ud->g = lua_tonumber( L, 3 ); }
				else if ( name == "b" ) { ud->b = lua_tonumber( L, 3 ); }
				else if ( name == "a" ) { ud->a = lua_tonumber( L, 3 ); }
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Color* ud = test_color( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDColor_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDColor_Name ": (r:%.3f, g:%.3f, b:%.3f, a:%.3f)", ud->r, ud->g, ud->b, ud->a );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_color_push( lua_State* L, Color color ) {
	Color* ud = (Color*) lua_newuserdata( L, sizeof( Color ) );
	*ud = color;

	state_color_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
