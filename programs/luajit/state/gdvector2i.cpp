#include "gdvector2i.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( vector2i, Vector2i* )

void state_vector2i_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDVector2i_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDVector2i_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector2i* ud = check_vector2i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				if ( name == "x" ) { lua_pushinteger( L, ud->x ); return 1; }
				if ( name == "y" ) { lua_pushinteger( L, ud->y ); return 1; }

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
			Vector2i* ud = check_vector2i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				if      ( name == "x" ) { ud->x = lua_tointeger( L, 3 ); }	
				else if ( name == "y" ) { ud->y = lua_tointeger( L, 3 ); }
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector2i* ud = test_vector2i( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDVector2i_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDVector2i_Name ": (x:%d, y:%d)", ud->x, ud->y );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_vector2i_push( lua_State* L, Vector2i vector2i ) {
	Vector2i* ud = (Vector2i*) lua_newuserdata( L, sizeof( Vector2i ) );
	*ud = vector2i;

	state_vector2i_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
