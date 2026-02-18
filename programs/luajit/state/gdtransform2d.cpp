#include "gdtransform2d.hpp"
#include "state.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( transform2d, Transform2D* )

void state_transform2d_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDTransform2D_Name ) ) {
		lua_pushstring( L, "__name" );
		lua_pushstring( L, GDTransform2D_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Transform2D* ud = check_transform2d( L, 1 );
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
			Transform2D* ud = check_transform2d( L, 1 );
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );

				// TODO
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Transform2D* ud = test_transform2d( L, 1 );
			if ( ud == NULL ) {
				lua_pushfstring( L, GDTransform2D_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDTransform2D_Name ": (%s)", ud );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );
	}
}

int state_transform2d_push( lua_State* L, Transform2D transform2d ) {
	Transform2D* ud = (Transform2D*) lua_newuserdata( L, sizeof( Transform2D ) );
	*ud = transform2d;

	state_transform2d_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
