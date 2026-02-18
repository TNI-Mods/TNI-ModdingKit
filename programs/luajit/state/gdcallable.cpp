#include "gdcallable.hpp"
#include "state.hpp"
#include "utils.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( callable, Callable* )

void state_callable_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDCallable_Name ) ) {
		lua_pushstring( L, "__name" );  // Was added in Lua 5.3, and not in LuaJIT, but it's nice to have anyway.
		lua_pushstring( L, GDCallable_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__call" );
		lua_pushcfunction(L, []( lua_State *L ) -> int {
			// Stack: GDCallable, args...
			Callable* ud = check_callable( L, 1 );
			Variant v = Variant( *ud );
			const int args_pos = 2;  // Position on stack where arguments start.
			int nargs = lua_gettop( L ) - ( args_pos - 1 );
			Variant result;
			switch ( nargs ) {
				case 0: 
					result = ud->call(); 
					break;
				case 1: 
					result = ud->call( state_lua_to_variant( L, args_pos ) ); 
					break;
				case 2: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ) 
					); 
					break;
				case 3: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ), 
						state_lua_to_variant( L, args_pos + 2 )
					); 
					break;
				case 4: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ), 
						state_lua_to_variant( L, args_pos + 2 ), 
						state_lua_to_variant( L, args_pos + 3 )
					); 
					break;
				case 5: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ), 
						state_lua_to_variant( L, args_pos + 2 ), 
						state_lua_to_variant( L, args_pos + 3 ), 
						state_lua_to_variant( L, args_pos + 4 )
					); 
					break;
				case 6: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ), 
						state_lua_to_variant( L, args_pos + 2 ), 
						state_lua_to_variant( L, args_pos + 3 ), 
						state_lua_to_variant( L, args_pos + 4 ), 
						state_lua_to_variant( L, args_pos + 5 )
					); 
					break;
				case 7: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ), 
						state_lua_to_variant( L, args_pos + 2 ), 
						state_lua_to_variant( L, args_pos + 3 ), 
						state_lua_to_variant( L, args_pos + 4 ), 
						state_lua_to_variant( L, args_pos + 5 ), 
						state_lua_to_variant( L, args_pos + 6 )
					); 
					break;
				case 8: 
					result = ud->call(
						state_lua_to_variant( L, args_pos ), 
						state_lua_to_variant( L, args_pos + 1 ), 
						state_lua_to_variant( L, args_pos + 2 ), 
						state_lua_to_variant( L, args_pos + 3 ), 
						state_lua_to_variant( L, args_pos + 4 ), 
						state_lua_to_variant( L, args_pos + 5 ), 
						state_lua_to_variant( L, args_pos + 6 ), 
						state_lua_to_variant( L, args_pos + 7 )
					); 
					break;
				// godot-sandbox only supports up to 8 args.
				// https://github.com/libriscv/godot-sandbox/blob/1553c9f988f9e174e843143a932f5ba488c9ccdc/src/sandbox_syscalls.cpp#L105
				// We could get around this by creating a godot array and using `ud->call("callv", args)`
				default:
					return luaL_error( L, GDCallable_Name ".__call Too many arguments" );  // luaL_error never returns.
			}
			return state_lua_push_variant( L, result );
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Callable* ud = test_callable( L, 1 );
			if (ud == NULL) {
				lua_pushfstring( L, GDCallable_Name ": INVALID");
			} else {
				lua_pushfstring( L, GDCallable_Name ": %d@%p", ud->get_variant_index(), ud );  // TODO: Can we get the name and/or signature?
			}
			return 1;
		});
		lua_settable( L, -3 );
	}
}

int state_callable_push( lua_State* L, Callable callable ) {
	// `Callable` just holds an index to the real callable.
	Callable* ud = (Callable*) lua_newuserdata( L, sizeof( Callable ) );
	*ud = callable;

	state_callable_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}

static Variant state_callable_function_lambda( uint64_t Lptr, Array args ) {
	lua_State* L = (lua_State*) Lptr;
	
	// Stack: function
	lua_settop( L, 1 );
	
	int nargs = 0;
	for ( int i = 0; i < args.size(); i++ ) {
		nargs += state_lua_push_variant( L, args[i] );
	}
	
	if ( pcall_stacktrace( L, nargs, 1 ) != 0 ) {
		const char *err = lua_tostring( L, -1 );
		printf( "Lua exec error: %s\n", err );
		lua_pop( L, 1 );
		throw std::runtime_error( "Lua exec error" );
	}
	
	if ( lua_gettop( L ) > 1 ) {
		return state_lua_to_variant( L, -1 );
	}
	return Nil;
}

Callable state_callable_from_function( lua_State* L, int pos ) {
	if ( ! lua_isfunction( L, pos ) ) return Nil;

	lua_getregistry( L );    // Stack: ..., LUA_REGISTRY
	lua_pushvalue( L, pos ); // Stack: ..., LUA_REGISTRY, function
	lua_gettable( L, -2 );   // Stack: ..., LUA_REGISTRY, value

	lua_State* co;
	if ( ! lua_isthread( L, -1 ) ) {
		lua_pop( L, 1 );
		// Create new lua thread (aka coroutine)
		co = lua_newthread( L ); // Stack: ..., LUA_REGISTRY, co
		// Put the function in the new thread's stack.
		lua_pushvalue( L, pos ); // Stack: ..., LUA_REGISTRY, co, function
		lua_xmove( L, co, 1 );   // Stack: ..., LUA_REGISTRY, co

		lua_pushvalue( L, pos ); // Stack: ..., LUA_REGISTRY, co
		lua_pushvalue( L, -2 );  // Stack: ..., LUA_REGISTRY, co, function
		lua_settable( L, -4 );   // Stack: ..., LUA_REGISTRY, co
		lua_pop( L, 2 );         // Stack: ...
	} else {
		co = lua_tothread( L, -1 ); // Stack: ..., LUA_REGISTRY, co
		lua_pop( L, 2 );            // Stack: ...
	}

	return get_node<Mod>().callable_args_to_array( 
		Callable::Create( state_callable_function_lambda, Variant( (uint64_t) co ) ) 
	);
}
