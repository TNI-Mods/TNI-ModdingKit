#include "gdobject.hpp"
#include "state.hpp"

VERIFY_MODE_WRAPPER( object, Object )

void state_object_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDObject_Name ) ) {
		lua_pushstring( L, "__name" );  // Was added in Lua 5.3, and not in LuaJIT, but it's nice to have anyway.
		lua_pushstring( L, GDObject_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Object obj = check_object( L, 1 );
			// Variant value = state_lua_to_variant(L, 2);
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				// We can't catch exceptions from the sandbox, so we check if we are allowed first.
				if ( ! ( (Mod) get_node() ).is_allowed_property( obj, name ) ) {
					return luaL_error( L, "Banned property accessed: %s", name.c_str() );  // This does not return!
				}
				// TODO: We could temporarily cache GDCallable userdata during this VM call.
				// TODO: GDNameCall instead of GDCallable to handle `obj:method()`, which can bypass GDCallable using obj.call/obj.callv (more performant and Lua idiomatic)
				
				return state_lua_push_variant( L, obj.get( name ) );
			}
			lua_pushnil( L );
			return 1;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__newindex" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			// Stack: GDObject, index, value
			Object obj = check_object( L, 1 );
			// Variant value = state_lua_to_variant(L, 2);
			if ( lua_isstring( L, 2 ) ) {
				std::string name = lua_tostring( L, 2 );
				Variant value = state_lua_to_variant( L, 3 );
				obj.set( name, value );
				return 0;
			}
			return luaL_error( L, GDObject_Name ".__newindex only supports string keys but got %s", luaL_typename( L, 2 ) );
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Object obj = test_object(L, 1);
			if ( ! obj.is_valid() ) {
				lua_pushfstring( L, GDObject_Name ": INVALID" );
			} else {
				String s = obj.to_string();
				lua_pushfstring( L, GDObject_Name ": '%s'", s.utf8().c_str() );
			}
			return 1;
		});
		lua_settable( L, -3 );
	}
}

int state_object_push( lua_State* L, Object object ) {
	if ( object.address() == 0 ) {
		lua_pushnil( L );
		return 1;
	}

	uint64_t* ud = (uint64_t*) lua_newuserdata( L, sizeof( uint64_t ) );
	*ud = uint64_t( object( "get_instance_id" ) );  // object.get_instance_id() wrongly returns int

	state_object_metatable( L );
	lua_setmetatable( L, -2 );
	return 1;
}
