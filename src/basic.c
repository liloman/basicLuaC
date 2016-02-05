/*
** basic C API for lua
** 
*/

#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define FALSE 0
#define TRUE  !FALSE
#if (LUA_VERSION_NUM == 501)

#define lua_pushglobaltable( L )    lua_pushvalue( L, LUA_GLOBALSINDEX )
#define luaL_newlib( L, funcs )     { lua_newtable( L ); \
  luaL_register( L, NULL, funcs ); }
#define isequal( L, a, b )          lua_equal( L, a, b )
#define requiref( L, modname, f, glob ) {\
  lua_pushcfunction( L, f ); /* push module load function */ \
  lua_pushstring( L, modname );  /* argument to module load function */ \
  lua_call( L, 1, 1 );  /* call 'f' to load module */ \
  /* register module in package.loaded table in case 'f' doesn't do so */ \
  lua_getfield( L, LUA_GLOBALSINDEX, LUA_LOADLIBNAME );\
  if ( lua_type( L, -1 ) == LUA_TTABLE ) {\
    lua_getfield( L, -1, "loaded" );\
    if ( lua_type( L, -1 ) == LUA_TTABLE ) {\
      lua_getfield( L, -1, modname );\
      if ( lua_type( L, -1 ) == LUA_TNIL ) {\
        lua_pushvalue( L, 1 );\
        lua_setfield( L, -3, modname );\
      }\
      lua_pop( L, 1 );\
    }\
    lua_pop( L, 1 );\
  }\
  lua_pop( L, 1 );\
  if ( glob ) { /* set global name? */ \
    lua_setglobal( L, modname );\
  } else {\
    lua_pop( L, 1 );\
  }\
}

#else

#define isequal( L, a, b )                 lua_compare( L, a, b, LUA_OPEQ )
#define requiref( L, modname, f, glob ) \
  { luaL_requiref( L, modname, f, glob ); lua_pop( L, 1 ); }

#endif


/********************
 * global variables *
 *******************/
static lua_Number test=4.1;


/***********************
 * register prototypes *
 ***********************/

// library functions
static int basic_foo( lua_State *L );

// register functions
// register this lib
LUALIB_API int luaopen_basic( lua_State *L );
static int basic_loadlib( lua_State *L ); 
// register all libs including this
static void basic_openlualibs( lua_State *L ); 

/***********
 * structs *
 ***********/

/* basic function registration array */
static const struct luaL_Reg basic_funcs[] = {
  { "foo", basic_foo },
  { NULL, NULL }
};


/***********************
*  library functions  *
***********************/

/* Print test static variable * arg1 */
static int basic_foo( lua_State *L ) {
    /* load standard libraries and basic */
    basic_openlualibs( L );  
    lua_pushnumber(L,(test * lua_tonumber(L,1) ));
  return 1;
}

/**********************************
 * register structs and functions *
 **********************************/

// Load shared library
LUALIB_API int luaopen_basic( lua_State *L ) {
    /* register basic functions onto a new lib table*/
    luaL_newlib( L, basic_funcs );
    return 1;
}

static void basic_reglualib( lua_State *L, const char *name, 
                               lua_CFunction f ) {
  lua_getglobal( L, "package" );
  lua_getfield( L, -1, "preload" );
  lua_pushcfunction( L, f );
  lua_setfield( L, -2, name );
  lua_pop( L, 2 );
}

static void basic_openlualibs( lua_State *L ) {
  /* register basic's own functions */
  requiref(L, "basic", basic_loadlib, TRUE );
  requiref( L, "_G", luaopen_base, FALSE );
  requiref( L, "package", luaopen_package, TRUE );
  basic_reglualib( L, "io", luaopen_io );
  basic_reglualib( L, "os", luaopen_os );
  basic_reglualib( L, "table", luaopen_table );
  basic_reglualib( L, "string", luaopen_string );
  basic_reglualib( L, "math", luaopen_math );
  basic_reglualib( L, "debug", luaopen_debug );
#if (LUA_VERSION_NUM == 502)
  basic_reglualib( L, "bit32", luaopen_bit32 );
#endif
#if (LUA_VERSION_NUM >= 502)
  basic_reglualib( L, "coroutine", luaopen_coroutine );
#endif
#if (LUA_VERSION_NUM >= 503)
  basic_reglualib( L, "utf8", luaopen_utf8 );
#endif

}

static int basic_loadlib( lua_State *L ) {

  /* register basic functions */
  luaL_newlib( L, basic_funcs );

  return 1;
}
