#include "gdquaternion.hpp"
#include "tower.hpp"
#include "state.hpp"

VERIFY_MODE_WRAPPER( quaternion, Quaternion* )

void state_quaternion_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDQuaternion_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDQuaternion_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Quaternion* ud = check_quaternion( L, 1 );
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
			Quaternion* ud = check_quaternion( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				// TODO
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Quaternion* ud = test_quaternion( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDQuaternion_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDQuaternion_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_quaternion_push( lua_State* L, Quaternion quaternion ) {
	Quaternion* ud = (Quaternion*) lua_newuserdata( L, sizeof( Quaternion ) );
	*ud = quaternion;

	state_quaternion_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
