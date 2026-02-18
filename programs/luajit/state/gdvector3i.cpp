#include "gdvector3i.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( vector3i, Vector3i* )

void state_vector3i_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDVector3i_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDVector3i_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector3i* ud = check_vector3i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				if ( name == "x" ) { lua_pushinteger( L, ud->x ); return 1; }
				if ( name == "y" ) { lua_pushinteger( L, ud->y ); return 1; }
				if ( name == "z" ) { lua_pushinteger( L, ud->z ); return 1; }

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
			Vector3i* ud = check_vector3i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				
				if      ( name == "x" ) { ud->x = lua_tointeger( L, 3 ); }	
				else if ( name == "y" ) { ud->y = lua_tointeger( L, 3 ); }
				else if ( name == "z" ) { ud->z = lua_tointeger( L, 3 ); }
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector3i* ud = test_vector3i( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDVector3i_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDVector3i_Name ": (x:%d, y:%d, z:%d)", ud->x, ud->y, ud->z );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_vector3i_push( lua_State* L, Vector3i vector3i ) {
	Vector3i* ud = (Vector3i*) lua_newuserdata( L, sizeof( Vector3i ) );
	*ud = vector3i;

	state_vector3i_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
