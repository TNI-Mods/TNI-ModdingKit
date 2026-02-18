#include "gdvector4i.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( vector4i, Vector4i* )

void state_vector4i_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDVector4i_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDVector4i_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector4i* ud = check_vector4i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				if ( name == "x" ) { lua_pushinteger( L, ud->x ); return 1; }
				if ( name == "y" ) { lua_pushinteger( L, ud->y ); return 1; }
				if ( name == "z" ) { lua_pushinteger( L, ud->z ); return 1; }
				if ( name == "w" ) { lua_pushinteger( L, ud->w ); return 1; }

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
			Vector4i* ud = check_vector4i( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				
				if      ( name == "x" ) { ud->x = lua_tointeger( L, 3 ); }	
				else if ( name == "y" ) { ud->y = lua_tointeger( L, 3 ); }
				else if ( name == "z" ) { ud->z = lua_tointeger( L, 3 ); }
				else if ( name == "w" ) { ud->w = lua_tointeger( L, 3 ); }
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector4i* ud = test_vector4i( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDVector4i_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDVector4i_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_vector4i_push( lua_State* L, Vector4i vector4i ) {
	Vector4i* ud = (Vector4i*) lua_newuserdata( L, sizeof( Vector4i ) );
	*ud = vector4i;

	state_vector4i_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
