/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */

#include <tarantool/module.h>

#include <lua.h>
#include <lauxlib.h>

#include "hyperloglog.h"

/**

   hyperloglog.new()
   hyperloglog.add(string)
   hyperloglog.count()
   hyperloglog.merge(hll)

 */

// identifier for the custom userdata type
static const char *UD_HLL = "hyperloglog";

LUA_API int
hyperloglog_add(lua_State *L)
{
  size_t len;
  HyperLogLog *hll = luaL_checkudata(L, 1, UD_HLL);
  const char *key = luaL_checklstring(L, 2, &len);

  hll_add(hll, (unsigned char *)key, len);

  return 0;
}

LUA_API int
hyperloglog_count(lua_State *L)
{
  HyperLogLog *hll = luaL_checkudata(L, 1, UD_HLL);

  lua_pushnumber(L, hll_count(hll));

  return 1;
}

LUA_API int
hyperloglog_serialise(lua_State *L)
{
  HyperLogLog *hll = luaL_checkudata(L, 1, UD_HLL);

  luaL_Buffer b;
  luaL_buffinit(L, &b);

  // hll->encoding
  luaL_addlstring(&b, (const char *)&hll->encoding, 1);

  // hll->registers
  luaL_addlstring(&b, (const char *)hll->registers, hll->mlen);

  luaL_pushresult(&b);

  return 1;
}

LUA_API int
hyperloglog_gc(lua_State *L)
{
  HyperLogLog *hll = luaL_checkudata(L, 1, UD_HLL);

  // @TODO: free bitmap

  return 0;
}

LUA_API int
hyperloglog_tostring(lua_State *L)
{
  HyperLogLog *hll = luaL_checkudata(L, 1, UD_HLL);

  lua_pushfstring(L, "HLL(%d)", hll_count(hll));

  return 1;
}

LUA_API int
hyperloglog_new(lua_State *L)
{
  HyperLogLog *hll = lua_newuserdata(L, sizeof(HyperLogLog));

  // @TODO: allocate bitmap
  hll_create(hll);

  luaL_getmetatable(L, UD_HLL);
  lua_setmetatable(L, -2);

  return 1;
}

LUA_API int
hyperloglog_fromstring(lua_State *L)
{
  size_t len;
  const char *s = luaL_checklstring(L, 1, &len);

  HyperLogLog *hll = lua_newuserdata(L, sizeof(HyperLogLog));

  // @TODO: allocate bitmap
  hll_create(hll);

  hll->encoding = s[0];
  hll->mlen = len - 1;
  memcpy(hll->registers, s + 1, len - 1);

  luaL_getmetatable(L, UD_HLL);
  lua_setmetatable(L, -2);

  return 1;
}

LUA_API int
luaopen_hyperloglog(lua_State *L)
{
  static const struct luaL_Reg methods[] = {
    {"add", hyperloglog_add},
    {"count", hyperloglog_count},
    {"serialise", hyperloglog_serialise},
    {"__gc", hyperloglog_gc},
    {"__tostring", hyperloglog_tostring},
    {NULL, NULL}
  };

  static const struct luaL_Reg meta[] = {
    {"new", hyperloglog_new},
    {"fromstring", hyperloglog_fromstring},
    {NULL, NULL}
  };

  luaL_newmetatable(L, UD_HLL);
  lua_pushvalue(L, -1);
  luaL_register(L, NULL, methods);
  lua_setfield(L, -2, "__index");
  lua_pushstring(L, UD_HLL);
  lua_setfield(L, -2, "__metatable");
  lua_pop(L, 1);

  lua_newtable(L);
  luaL_register(L, NULL, meta);

  return 1;
}
