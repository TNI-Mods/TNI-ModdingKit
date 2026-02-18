#include "state.hpp"
#include "luamod.hpp"
#include "tower.hpp"

#include "state/gd.hpp"

EXTERN_SYSCALL( uint64_t, sys_node_create, Node_Create_Shortlist, const char *, size_t, const char *, size_t );

void state_setup( lua_State* L ) {

	Mod mod = get_node<Mod>( "." );
	STATE_SETUP_OBJECT( L, Mod, mod )

	ModApiV1 modapiV1 = mod.api_v1();
	STATE_SETUP_OBJECT( L, ModApiV1, modapiV1 )

	ModFileSystem modfs = mod.filesystem();
	STATE_SETUP_OBJECT( L, ModFileSystem, modfs )

	Engine engine = Engine::get_singleton();
	STATE_SETUP_OBJECT( L, Engine, engine )

	lua_pushcfunction( L, []( lua_State *L ) -> int {
		std::string classname = luaL_checkstring( L, 1 );
		std::string path = luaL_checkstring( L, 2 );
		return state_sol_lua_push_gd(
			L, 
			Object(
				sys_node_create(
					Node_Create_Shortlist::CREATE_CLASSDB, 
					classname.data(), 
					classname.size(), 
					path.data(), 
					path.size()
				)
			)
		);
	});
	lua_setglobal( L, "create_node" );

	lua_getglobal( L, "package" );
	// Stack: package
	lua_pushstring( L, "mod://?.lua;mod://?/init.lua" );
	// Stack: package, string
	lua_setfield( L, -2, "path" );
	// Stack: package
	lua_pushcfunction( L, luamod_search_path );
	// Stack: package, luamod_search_path
	lua_setfield( L, -2, "searchpath" );
	// Stack: package
	lua_newtable( L );
	// Stack: package, table
	lua_pushcfunction( L, luamod_searcher_lua );
	// Stack: package, table, luamod_searcher_lua
	lua_rawseti ( L, -2, lua_objlen( L, -2 ) + 1 );
	// Stack: package, table
	lua_setfield( L, -2, "loaders" );
	// Stack: package
	lua_pop( L, 1 );
	// Stack:
}

int state_lua_push_variant( lua_State *lua_state, const Variant& value, const char* global_name ) {
	int result = 0;

	switch ( value.get_type() ) {
		case Variant::Type::NIL: result = sol::stack::push( lua_state, sol::nil ); break;

		// -----> ATOMIC TYPES
		case Variant::Type::BOOL:   STACK_PUSH_SIMPLE( lua_state, (bool) value,             global_name, result ) break;
		case Variant::Type::INT:    STACK_PUSH_SIMPLE( lua_state, (int64_t) value,          global_name, result ) break;
		case Variant::Type::FLOAT:  STACK_PUSH_SIMPLE( lua_state, (double) value,           global_name, result ) break;
		case Variant::Type::STRING: STACK_PUSH_SIMPLE( lua_state, value.as_string().utf8(), global_name, result ) break;

		// -----> MATH TYPES
		case Variant::Type::VECTOR2:     STACK_PUSH_COMPLEX( lua_state, (Vector2) value,     global_name, result, state_vector2_push     ) break;
		case Variant::Type::VECTOR2I:    STACK_PUSH_COMPLEX( lua_state, (Vector2i) value,    global_name, result, state_vector2i_push    ) break;
		case Variant::Type::RECT2:       STACK_PUSH_COMPLEX( lua_state, (Rect2) value,       global_name, result, state_rect2_push       ) break;
		case Variant::Type::RECT2I:      STACK_PUSH_COMPLEX( lua_state, (Rect2i) value,      global_name, result, state_rect2i_push      ) break;
		case Variant::Type::VECTOR3:     STACK_PUSH_COMPLEX( lua_state, (Vector3) value,     global_name, result, state_vector3_push     ) break;
		case Variant::Type::VECTOR3I:    STACK_PUSH_COMPLEX( lua_state, (Vector3i) value,    global_name, result, state_vector3i_push    ) break;
		case Variant::Type::TRANSFORM2D: STACK_PUSH_COMPLEX( lua_state, (Transform2D) value, global_name, result, state_transform2d_push ) break;
		case Variant::Type::VECTOR4:     STACK_PUSH_COMPLEX( lua_state, (Vector4) value,     global_name, result, state_vector4_push     ) break;
		case Variant::Type::VECTOR4I:    STACK_PUSH_COMPLEX( lua_state, (Vector4i) value,    global_name, result, state_vector4i_push    ) break;
		case Variant::Type::QUATERNION:  STACK_PUSH_COMPLEX( lua_state, (Quaternion) value,  global_name, result, state_quaternion_push  ) break;
		case Variant::Type::TRANSFORM3D: STACK_PUSH_COMPLEX( lua_state, (Transform3D) value, global_name, result, state_transform3d_push ) break;

		// -----> MISC TYPES
		case Variant::Type::COLOR:       STACK_PUSH_COMPLEX( lua_state, (Color) value,            global_name, result, state_color_push ) break;
		case Variant::Type::STRING_NAME: STACK_PUSH_SIMPLE(  lua_state, value.as_string().utf8(), global_name, result ) break;
		case Variant::Type::NODE_PATH:   STACK_PUSH_SIMPLE(  lua_state, value.as_string().utf8(), global_name, result ) break;
		case Variant::Type::OBJECT:      STACK_PUSH_COMPLEX( lua_state, value.as_object(),        global_name, result, state_object_push   ) break;
		case Variant::Type::CALLABLE:    STACK_PUSH_COMPLEX( lua_state, value.as_callable(),      global_name, result, state_callable_push ) break;
		case Variant::Type::ARRAY:       STACK_PUSH_COMPLEX( lua_state, value.as_array(),         global_name, result, state_array_push    ) break;
		
		default:
			printf( "state_lua_push_variant() Unhandled variant type %d\n", value.get_type() );
	}

	// sol::stack_object( lua_state, -1 );
	return result;
}

Variant state_lua_to_variant( lua_State* L, int pos ) {
	int luaType = lua_type( L, pos );
	switch ( luaType ) {
		case LUA_TNIL: return Nil;
		case LUA_TBOOLEAN: return Variant( lua_toboolean( L, pos ) );
		case LUA_TNUMBER:  return Variant( lua_tonumber( L, pos ) );
		case LUA_TSTRING:  size_t strLen; return Variant( std::string( lua_tolstring( L, pos, &strLen ), strLen ) );
		case LUA_TUSERDATA:
			// -----> MATH TYPES
			if ( Vector2* ud = state_vector2_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Vector2i* ud = state_vector2i_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Rect2* ud = state_rect2_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Rect2i* ud = state_rect2i_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Vector3* ud = state_vector3_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Vector3i* ud = state_vector3i_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Transform2D* ud = state_transform2d_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Vector4* ud = state_vector4_verify<VerifyMode::Test>( L, pos ) ) { return *ud;	}
			if ( Vector4i* ud = state_vector4i_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Quaternion* ud = state_quaternion_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Transform3D* ud = state_transform3d_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			
			// -----> MISC TYPES
			if ( Color* ud    = state_color_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Object obj   = state_object_verify<VerifyMode::Test>( L, pos ); obj.is_valid() ) { return obj; }
			if ( Callable* ud = state_callable_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }
			if ( Array* ud    = state_array_verify<VerifyMode::Test>( L, pos ) ) { return *ud; }

			luaL_error( L, "cannot convert userdata to godot variant" );
			return Nil;
		case LUA_TFUNCTION:
			return state_callable_from_function( L, pos );
		default:
			luaL_traceback(L, L, NULL, 0);
			const char* tb = lua_tostring( L, -1 );
			lua_pop( L, 1 );
			printf( "state_lua_to_variant() Unhandled lua type: %s\n%s\n", lua_typename( L, luaType ), tb );
			break;
	}
	return Nil;
}

#pragma region GODOT_TYPES

	STATE_LUA_PUSH_GODOT( const Array& )
	STATE_LUA_PUSH_GODOT( const Basis& )
	STATE_LUA_PUSH_GODOT( const Callable& )
	STATE_LUA_PUSH_GODOT( const Color& )
	STATE_LUA_PUSH_GODOT( const Dictionary& )
	STATE_LUA_PUSH_GODOT( const InputEvent& )
	STATE_LUA_PUSH_GODOT( const Node& )
	STATE_LUA_PUSH_GODOT( const Node2D& )
	STATE_LUA_PUSH_GODOT( const Node3D& )
	STATE_LUA_PUSH_GODOT( const NodePath& )
	STATE_LUA_PUSH_GODOT( Object )
	// STATE_LUA_PUSH_GODOT( const PackedByteArray& ) // Doesn't work 
	STATE_LUA_PUSH_GODOT( const PackedColorArray& )
	STATE_LUA_PUSH_GODOT( const PackedFloat32Array& )
	STATE_LUA_PUSH_GODOT( const PackedFloat64Array& )
	STATE_LUA_PUSH_GODOT( const PackedInt32Array& )
	STATE_LUA_PUSH_GODOT( const PackedInt64Array& )
	STATE_LUA_PUSH_GODOT( const PackedStringArray& )
	STATE_LUA_PUSH_GODOT( const PackedVector2Array& )
	STATE_LUA_PUSH_GODOT( const PackedVector3Array& )
	STATE_LUA_PUSH_GODOT( const PackedVector4Array& )
	STATE_LUA_PUSH_GODOT( const Plane& )
	STATE_LUA_PUSH_GODOT( const Quaternion& )
	STATE_LUA_PUSH_GODOT( const Rect2& )
	STATE_LUA_PUSH_GODOT( const Rect2i& )
	STATE_LUA_PUSH_GODOT( const RID& )
	STATE_LUA_PUSH_GODOT( const Transform2D& )
	STATE_LUA_PUSH_GODOT( const Transform3D& )
	STATE_LUA_PUSH_GODOT( const Variant& )
	STATE_LUA_PUSH_GODOT( const Vector2& )
	STATE_LUA_PUSH_GODOT( const Vector2i& )
	STATE_LUA_PUSH_GODOT( const Vector3& )
	STATE_LUA_PUSH_GODOT( const Vector3i& )
	STATE_LUA_PUSH_GODOT( const Vector4& )
	STATE_LUA_PUSH_GODOT( const Vector4i& )

#pragma endregion

#pragma region STD_TYPES

	STATE_LUA_PUSH_STD( bool )
	STATE_LUA_PUSH_STD( int64_t )
	STATE_LUA_PUSH_STD( double )
	STATE_LUA_PUSH_STD( String )

#pragma endregion
