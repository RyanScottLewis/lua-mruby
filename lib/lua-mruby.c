#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/compile.h"




static int lua_mruby_run(lua_State *L) {
  mrb_state *mrb = mrb_open();
  const char *mrb_code = lua_tostring(L, 1);
  struct mrb_parser_state *p = mrb_parse_string(mrb, mrb_code, NULL);
  int mrb_generated_code = mrb_generate_code(mrb, p-> tree);
  
  mrb_run( mrb, mrb_proc_new(mrb, mrb->irep[mrb_generated_code]), mrb_nil_value() );
  
  int lua_return = mrb_generated_code > 0 ? 0 : 1;
  lua_pushnumber(L, lua_return);
  
  mrb_close(mrb);
  
  return 1;
}

static const struct luaL_reg mrubylib[] = {
  { "run", lua_mruby_run },
  { NULL, NULL }
};





static int lua_mrb_open(lua_State *L) {
  mrb_state *mrb = mrb_open();
  
  lua_pushlightuserdata(L, &mrb);
  
  return 1;
}

static int lua_mrb_close(lua_State *L) {
  mrb_state *mrb = lua_topointer(L, 1); // Convert the first argument to a C pointer
  
  mrb_close(mrb);
  
  return 1;
}

static const struct luaL_reg mrblib[] = {
  { "mrbOpen" , lua_mrb_open  },
  { "mrbClose", lua_mrb_close },
  { NULL, NULL }
};





int luaopen_mruby(lua_State *L) {
  // luaL_register(L, NULL, mrblib);
  lua_register(L, "mrbOpen", lua_mrb_open);
  lua_register(L, "mrbClose", lua_mrb_close);
  
  luaL_register(L, "mruby", mrubylib);
  return 1;
}
