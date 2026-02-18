#include "gdvector3.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( vector3, Vector3* )

void state_vector3_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDVector3_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDVector3_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector3* ud = check_vector3( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				if ( name == "x" ) { lua_pushnumber( L, ud->x ); return 1; }
				if ( name == "y" ) { lua_pushnumber( L, ud->y ); return 1; }
				if ( name == "z" ) { lua_pushnumber( L, ud->z ); return 1; }

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
			Vector3* ud = check_vector3( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				
				if      ( name == "x" ) { ud->x = lua_tonumber( L, 3 ); }	
				else if ( name == "y" ) { ud->y = lua_tonumber( L, 3 ); }
				else if ( name == "z" ) { ud->z = lua_tonumber( L, 3 ); }
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector3* ud = test_vector3( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDVector3_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDVector3_Name ": (x:%.3f, y:%.3f, z:%.3f)", ud->x, ud->y, ud->z );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_vector3_push( lua_State* L, Vector3 vector3 ) {
	Vector3* ud = (Vector3*) lua_newuserdata( L, sizeof( Vector3 ) );
	*ud = vector3;

	state_vector3_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
