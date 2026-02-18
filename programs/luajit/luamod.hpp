#pragma once

#include <api.hpp>
#include <lua.hpp>

Variant luamod_set_source( lua_State* L, String code, String path );
int luamod_search_path( lua_State* L );
int luamod_searcher_lua( lua_State* L );
