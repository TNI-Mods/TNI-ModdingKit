#pragma once

enum class VerifyMode { Check, Test };

#define VERIFY_MODE_FUNCTION( name, type, mode, mode_l ) \
	type mode_l##_##name( lua_State* L, int pos ) { \
		return state_##name##_verify<VerifyMode::mode>( L, pos ); \
	}

#define VERIFY_MODE_WRAPPER( name, type ) \
	VERIFY_MODE_FUNCTION( name, type, Check, check ) \
	VERIFY_MODE_FUNCTION( name, type, Test, test )
