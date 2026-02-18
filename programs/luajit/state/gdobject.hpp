#pragma once

#include <api.hpp>
#include <lua.hpp>
#include "tower.hpp"

#include "mode.hpp"

#define GDObject_Name "GDObject"

template<VerifyMode mode>
Object state_object_verify( lua_State* L, int pos ){
	uint64_t* ud = (uint64_t*) luaL_testudata( L, pos, GDObject_Name );
	Variant v;

	if constexpr ( mode == VerifyMode::Check ) {
		v = get_node<Mod>().instance_from_id_( *ud );
		if ( v.get_type() == Variant::Type::NIL || ! v.as_object().is_valid() ) {
			luaL_error( L, "Attempt to index object that is no longer valid." );  // luaL_error never returns.
			return Nil;
		}
	} else {
		if (ud == NULL) { return Object( 0 ); }
		v = get_node<Mod>().instance_from_id_( *ud );
	}
	return v.as_object();
}

void state_object_metatable( lua_State* L );
int state_object_push( lua_State* L, Object object );
