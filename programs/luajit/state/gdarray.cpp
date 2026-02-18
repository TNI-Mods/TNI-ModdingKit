#include "gdarray.hpp"
#include "state.hpp"
#include "utils.hpp"
#include "tower.hpp"

VERIFY_MODE_WRAPPER( array, Array* )

void state_array_metatable( lua_State* L ) {
	if ( luaL_newmetatable( L, GDArray_Name )) {
		lua_pushstring( L, "create" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			return state_lua_push_variant( L, Array::Create() );
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "iter" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			// Stack: gdarray
			check_array( L, 1 );
			lua_pushcfunction( L, []( lua_State *L ) -> int {
				// Stack: gdarray, i
				Array* array = check_array( L, 1 );
				int i = lua_tointeger( L, 2 ) + 1;
				if ( i >= array->size() )
					return 0;
				lua_pushinteger( L, i );
				// Stack: gdarray, i, i+1
				Variant value = array->at( i );
				int nvalue = state_lua_push_variant( L, value );
				// Stack: gdarray, i, i+1, value
				return 1+nvalue;
			});
			// Stack: gdarray, iterator
			lua_insert( L, -2 );
			// Stack: iterator, gdarray
			lua_pushinteger( L, -1 );
			// Stack: iterator, gdarray, -1
			return 3;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__name" );  // Was added in Lua 5.3, and not in LuaJIT, but it's nice to have anyway.
		lua_pushstring( L, GDArray_Name );
		lua_settable( L, -3 );

		lua_pushstring( L, "__index" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			// Stack: gdarray, index
			Array array = *check_array( L, 1 );
			if ( lua_isnumber( L, 2 )) {
				lua_Number n = lua_tonumber( L, 2 );
				if ( std::fmod( n, 1 ) > 0 )
					luaL_argerror( L, 2, "expected integer but got number" );  // Never returns.
				int i = ( int )n;
				int size = array.size();
				int iabs = i < 0 ? size + i : i;  // array gets weird after trying to use negative indicies, godot-sandbox things ¯\_(ツ)_/¯
				if ( iabs < 0 || iabs >= size ) {
					return luaL_error( L, GDArray_Name " index (%d) out of bounds (%d elements)", i, size-1 );  // Never returns.
				}
				if ( ! ( (Mod) get_node() ).array_value_allowed( array, iabs ) ) {
					return luaL_error( L, "Object is not allowed: " GDArray_Name "[%d]", iabs );  // This does not return!
				}
				ArrayProxy elem = array.operator[]( iabs );
				return state_lua_push_variant( L, elem.get() );
			} else if ( lua_isstring( L, 2 ) ) {
				const char* key = luaL_checkstring( L, 2 );
				lua_getmetatable( L, 1 );
				// Stack: gdarray, index, GDArray_Name
				lua_getfield( L, -1, key );
				// Stack: gdarray, index, GDArray_Name, value
				if (!lua_isnil( L, -1 ))
					return 1;
				lua_pop( L, 2 );
				// Stack: gdarray, index
				lua_pushcclosure( L, state_array_variant_self_call, 1 );
				return 1;
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__newindex" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			// Stack: gdarray, index, value
			Array* array = check_array( L, 1 );
			int i = luaL_checkint( L, 2 );
			Variant value = state_lua_to_variant( L, 3 );
			int size = array->size();

			if ( i < 0 || i > size )
				return luaL_error( L, GDArray_Name " new index (%d) out of bounds (0-%d+1)", i, size-1 );  // Never returns.
			if ( i == size ) {
				array->push_back( value );
			} else {
				ArrayProxy elem = array->operator[]( i );
				elem.operator=( value );
			}
			return 0;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__len" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			// Stack: gdarray
			Array* array = check_array( L, 1 );
			lua_pushinteger( L, array->size() );
			return 1;
		});
		lua_settable( L, -3 );

		lua_pushstring( L, "__tostring" );
		lua_pushcfunction( L, []( lua_State *L ) -> int {
			Array* array = test_array( L, 1 );
			if ( array == NULL ) {
				lua_pushfstring( L, GDArray_Name ": INVALID" );
			} else {
				lua_pushfstring( L, GDArray_Name ": %d@%p-%s", array->get_variant_index(), array, array->is_permanent() ? "perm" : "temp" );
			}
			return 1;
		});
		lua_settable( L, -3 );
	}
}

int state_array_variant_self_call(lua_State *L) {
	// FIXME: There is no way to know with `v.callp` if the method actually exist or not...
	//        Can we find a way?

	// Stack: Variant, args...
	if (lua_type(L, 1) != LUA_TUSERDATA) {
		luaL_argerror(L, 1, "no indexable godot value was provided (did you use `.` instead of `:`?)");
	}

	Variant v = state_lua_to_variant(L, 1);
	if (v == Nil) {
		luaL_argerror(L, 1, "no indexable godot value was provided (did you use `.` instead of `:`?)");
	}

	const char* method = lua_tostring(L, lua_upvalueindex(1));

	const int args_pos = 2;  // Position on stack where arguments start.
	int nargs = lua_gettop(L) - (args_pos-1);
	if (nargs > 8)
		return luaL_error(L, "godot-sandbox only supports up to 8 arguments");

	if (nargs == 0) {
		Variant result;
		v.callp(method, nullptr, 0, result);
		return state_lua_push_variant(L, result);
	}
	std::array<Variant,8> vargs;
	for (int i = 0; i < nargs; i++) {
		vargs[i] = state_lua_to_variant(L, args_pos+i);
	}
	Variant result;
	v.callp(method, vargs.data(), nargs, result);
	return state_lua_push_variant(L, result);

	// Array args = Array::Create();
	// for (int i = 0; i < nargs; i++) {
	//     args.push_back(to_gd_variant(L, args_pos+i));
	// }
	
	// return push_gd_variant(L, v.method_call(name, args));
}

int state_array_push( lua_State* L, Array array ) {
	// `Array` just holds an index to the real array.
	Array* ud = (Array*) lua_newuserdata( L, sizeof( Array ) );
	// `Array` has assign operator, so we use memcpy to get around it.
	memcpy( ud, &array, sizeof( Array ) );

	state_array_metatable( L );
	lua_setmetatable( L, -2 );

	return 1;
}
