#include <lua.hpp>

#include "entry.hpp"
#include "luamod.hpp"
#include "state.hpp"

DEFINE_LUA_CALLBACK_0( on_mod_load )
DEFINE_LUA_CALLBACK_0( on_engine_load )
DEFINE_LUA_CALLBACK_0( on_game_state_ready )
DEFINE_LUA_CALLBACK_0( on_game_host_eod )

DEFINE_LUA_CALLBACK_1( on_game_tick, double, delta )
DEFINE_LUA_CALLBACK_1( on_player_input, InputEvent, event )
DEFINE_LUA_CALLBACK_1( on_device_spawned, Node, device )
DEFINE_LUA_CALLBACK_1( on_user_spawned, Node, user )
DEFINE_LUA_CALLBACK_1( on_location_spawned, Node, location )

Variant set_lua_source( String code, String path ) { 
	return luamod_set_source( lstate, code, path ); 
}

int main() {

	// stdout line buffering, to match TNI mod output buffering.
	setvbuf( stdout, NULL, _IOLBF, BUFSIZ );

	printf( "Setting up LUA state\n" );
	lstate = luaL_newstate(); luaL_openlibs( lstate );
	state_setup( lstate );

	// Only called for luajit.elf
	ADD_API_FUNCTION( set_lua_source, "", "");

	ADD_API_FUNCTION( on_mod_load, "", "", "" );
	ADD_API_FUNCTION( on_engine_load, "", "", "" );
	ADD_API_FUNCTION( on_game_state_ready, "", "", "" );
	ADD_API_FUNCTION( on_game_host_eod, "", "", "" );

	ADD_API_FUNCTION( on_game_tick, "", "", "" );
	ADD_API_FUNCTION( on_player_input, "", "", "" );
	ADD_API_FUNCTION( on_device_spawned, "", "", "" );
	ADD_API_FUNCTION( on_user_spawned, "", "", "" );
	ADD_API_FUNCTION( on_location_spawned, "", "", "" );

	halt();

}
