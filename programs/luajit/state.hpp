#pragma once

#include <api.hpp>
#include <lua.hpp>

#include <sol/sol.hpp>

void state_setup( lua_State* lua_state );

int state_lua_push_variant( lua_State* lua_state, const Variant& value, const char* global_name = "" );
Variant state_lua_to_variant( lua_State* L, int pos );

#pragma region GODOT_TYPES

	#define STATE_LUA_PUSH_GODOT( type ) \
		int state_sol_lua_push_gd( lua_State* L, type v ) { return state_lua_push_variant( L, Variant( v ), "" ); }
	#define DEF_STATE_LUA_PUSH_GODOT( type ) \
		int state_sol_lua_push_gd( lua_State* L, type v );

	DEF_STATE_LUA_PUSH_GODOT( const Array& )
	DEF_STATE_LUA_PUSH_GODOT( const Basis& )
	DEF_STATE_LUA_PUSH_GODOT( const Callable& )
	DEF_STATE_LUA_PUSH_GODOT( const Color& )
	DEF_STATE_LUA_PUSH_GODOT( const Dictionary& )
	DEF_STATE_LUA_PUSH_GODOT( const InputEvent& )
	DEF_STATE_LUA_PUSH_GODOT( const Node& )
	DEF_STATE_LUA_PUSH_GODOT( const Node2D& )
	DEF_STATE_LUA_PUSH_GODOT( const Node3D& )
	DEF_STATE_LUA_PUSH_GODOT( const NodePath& )
	DEF_STATE_LUA_PUSH_GODOT( Object )
	// DEF_STATE_LUA_PUSH_GODOT( const PackedByteArray& ) // Doesn't work 
	DEF_STATE_LUA_PUSH_GODOT( const PackedColorArray& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedFloat32Array& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedFloat64Array& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedInt32Array& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedInt64Array& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedStringArray& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedVector2Array& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedVector3Array& )
	DEF_STATE_LUA_PUSH_GODOT( const PackedVector4Array& )
	DEF_STATE_LUA_PUSH_GODOT( const Plane& )
	DEF_STATE_LUA_PUSH_GODOT( const Quaternion& )
	DEF_STATE_LUA_PUSH_GODOT( const Rect2& )
	DEF_STATE_LUA_PUSH_GODOT( const Rect2i& )
	DEF_STATE_LUA_PUSH_GODOT( const RID& )
	DEF_STATE_LUA_PUSH_GODOT( const Transform2D& )
	DEF_STATE_LUA_PUSH_GODOT( const Transform3D& )
	DEF_STATE_LUA_PUSH_GODOT( const Variant& )
	DEF_STATE_LUA_PUSH_GODOT( const Vector2& )
	DEF_STATE_LUA_PUSH_GODOT( const Vector2i& )
	DEF_STATE_LUA_PUSH_GODOT( const Vector3& )
	DEF_STATE_LUA_PUSH_GODOT( const Vector3i& )
	DEF_STATE_LUA_PUSH_GODOT( const Vector4& )
	DEF_STATE_LUA_PUSH_GODOT( const Vector4i& )

#pragma endregion

#pragma region STD_TYPES

	#define STATE_LUA_PUSH_STD( type ) \
		int state_sol_lua_push_gd( lua_State* L, type &v ) { return state_lua_push_variant( L, Variant( v ), "" ); }
	#define DEF_STATE_LUA_PUSH_STD( type ) \
		int state_sol_lua_push_gd( lua_State* L, type &v );

	DEF_STATE_LUA_PUSH_STD( bool )
	DEF_STATE_LUA_PUSH_STD( int64_t )
	DEF_STATE_LUA_PUSH_STD( double )
	DEF_STATE_LUA_PUSH_STD( String )

#pragma endregion

#define STATE_SETUP_OBJECT( L, type, value ) \
	if ( state_lua_push_variant( L, value, #type ) != 1 ) { \
		luaL_error( L, "Failed to push " #type " object." ); \
	}

#define STACK_PUSH_SIMPLE( var_state, var_value, var_global, var_result ) \
	STACK_PUSH_COMPLEX( var_state, var_value, var_global, var_result, sol::stack::push )

#define STACK_PUSH_COMPLEX( var_state, var_value, var_global, var_result, fn_push ) \
	var_result = fn_push( var_state, var_value ); \
	if ( result == 1 && var_global && var_global[0] != '\0' ) { \
		lua_setglobal( var_state, var_global ); \
	}
