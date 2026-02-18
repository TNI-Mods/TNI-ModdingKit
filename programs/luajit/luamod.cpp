#include "luamod.hpp"

#include <api.hpp>
#include <cstring>

#include "entry.hpp"
#include "tower.hpp"


// =====> NOT IN HEADER <============================================================================================================================

// Copied and modified from luajit:lib_package.c:pushnexttemplate
static const char* luamod_internal_push_next_template( lua_State *L, const char *path ) {
	const char *l;
	while ( *path == *LUA_PATHSEP ) path++;  /* skip separators */
	if ( *path == '\0' ) { return NULL; } /* no more templates */
	l = strchr( path, *LUA_PATHSEP );  /* find next separator */
	if ( l == NULL) l = path + strlen( path );
	lua_pushlstring( L, path, (size_t)( l - path ) );  /* template */
	return l;
}

// Based on version from luajit:lib_package.c:readable
static bool luamod_internal_mod_readable( const char *path ) {
	Mod mod = get_node<Mod>();
	ModFileSystem modfs = (ModFileSystem) mod.filesystem();
	std::optional<ModFileAccess> modfa = modfs.open( String( std::string_view( path ) ), 1 );
	if ( ! modfa.has_value() ) { return false; }
	modfa.value().close();
	return true;
}

// Copied and modified from luajit:lib_package.c:searchpath
static const char* luamod_internal_search_path( lua_State *L, const char *name, const char *path, const char *sep, const char *dirsep ) {
	// TODO: If name begins with absolute ("mod://" or "mods://"), then ignore path and only check absolute path.
	luaL_Buffer msg;  /* to build error message */
	luaL_buffinit( L, &msg );
	if ( *sep != '\0' ) { /* non-empty separator? */
		name = luaL_gsub(L, name, sep, dirsep);  /* replace it by 'dirsep' */
	}
	while ( ( path = luamod_internal_push_next_template( L, path ) ) != NULL ) {
		const char *filename = luaL_gsub( L, lua_tostring( L, -1 ), LUA_PATH_MARK, name );
		lua_remove( L, -2 );  /* remove path template */
		if ( luamod_internal_mod_readable( filename ) ) { /* does file exist and is readable? */
			return filename;  /* return that file name */
		}
		lua_pushfstring( L, "\n\tno file " LUA_QS, filename );
		lua_remove( L, -2 );  /* remove file name */
		luaL_addvalue( &msg );  /* concatenate error msg. entry */
	}
	luaL_pushresult( &msg );  /* create error message */
	return NULL;  /* not found */
}

// Copied and modified from luajit:lib_package.c:findfile
static const char* luamod_internal_find_file( lua_State *L, const char *name, const char *pname ) {
	const char *path;
	lua_getglobal( L, "package" );
	lua_getfield( L, -1, pname );
	lua_remove( L, -2 );
	path = lua_tostring( L, -1 );
	if (path == NULL) { luaL_error( L, LUA_QL( "package.%s" ) " must be a string", pname ); }
	return luamod_internal_search_path( L, name, path, ".", LUA_DIRSEP );
}



// =====> IN HEADER <================================================================================================================================

Variant luamod_set_source( lua_State* L, String code, String path ) {
	if ( L == NULL ) {
		printf( "`luamod_set_source()` but the lua state == NULL\n" );
		return Nil;
	}

	const std::string src = code.utf8();
	const std::string name = "@" + path.utf8();
	if ( luaL_loadbuffer( L, src.c_str(), src.size(), name.c_str() ) != 0 ) {
		const char *err = lua_tostring( L, -1 );
		printf( "Lua load error: %s\n", err );
		lua_pop( L, 1 );
		return Nil;
	}
	
	if ( pcall_stacktrace( L, 0, 0 ) != 0 ) {
		const char *err = lua_tostring( L, -1 );
		printf( "Lua exec error: %s\n", err );
		lua_pop( L, 1 );
		return Nil;
	}
	
	return Nil;
}

// Copied and modified from luajit:lua_modsearchpath
int luamod_search_path( lua_State* L ) {
	const char* f = luamod_internal_search_path(
		L,
		luaL_checkstring( L, 1 ),
		luaL_checkstring( L, 2 ),
		luaL_optstring( L, 3, "." ),
		luaL_optstring( L, 4, LUA_DIRSEP )
	);
	if ( f != NULL ) { return 1; }

	/* error message is on top of the stack */
	lua_pushnil( L );
	lua_insert( L, -2 );
	return 2; /* return nil + error message */
}

int luamod_searcher_lua( lua_State* L ) {
	// FIXME: Due to using `findfile`, we make 2 calls into godot to try open the file, once to check readable and the 2nd here.
	const char *filename;
	const char *name = luaL_checkstring( L, 1 );
	filename = luamod_internal_find_file( L, name, "path" );
	if (filename == NULL) { return 1; } /* library not found in this path */
	Mod mod = get_node<Mod>();
	ModFileSystem modfs = (ModFileSystem) mod.filesystem();
	ModFileAccess modfa = modfs.open( String( std::string_view( filename ) ), 1 ).value();
	String src = modfa.get_as_text( modfa.get_length() );
	std::string src_name = std::string() + "@" + name;
	if (luaL_loadbuffer( L, src.utf8().c_str(), src.size(), src_name.c_str()) != 0 ) {
		luaL_error( L, "error loading module " LUA_QS " from file " LUA_QS ":\n\t%s", name, filename, lua_tostring( L, -1 ) );
	}
	return 1;  /* library loaded successfully */
}
