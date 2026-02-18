#include "gdvector4.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( vector4, Vector4* )

void state_vector4_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDVector4_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDVector4_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector4* ud = check_vector4( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				if ( name == "x" ) { lua_pushnumber( L, ud->x ); return 1; }
				if ( name == "y" ) { lua_pushnumber( L, ud->y ); return 1; }
				if ( name == "z" ) { lua_pushnumber( L, ud->z ); return 1; }
				if ( name == "w" ) { lua_pushnumber( L, ud->w ); return 1; }

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
			Vector4* ud = check_vector4( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				
				if      ( name == "x" ) { ud->x = lua_tonumber( L, 3 ); }	
				else if ( name == "y" ) { ud->y = lua_tonumber( L, 3 ); }
				else if ( name == "z" ) { ud->z = lua_tonumber( L, 3 ); }
				else if ( name == "w" ) { ud->w = lua_tonumber( L, 3 ); }
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Vector4* ud = test_vector4( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDVector4_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDVector4_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_vector4_push( lua_State* L, Vector4 vector4 ) {
	Vector4* ud = (Vector4*) lua_newuserdata( L, sizeof( Vector4 ) );
	*ud = vector4;

	state_vector4_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
