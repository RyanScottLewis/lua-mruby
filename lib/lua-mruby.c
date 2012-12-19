#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/compile.h"

static int lua_mruby_run(lua_State *L) {
  // mrb_state *mrb = mrb_open();
  // const char *mrb_code = lua_tostring(L, 1);
  // struct mrb_parser_state* parser = mrb_parse_string(mrb, mrb_code, NULL);
  // int mrb_generated_code = mrb_generate_code(mrb, parser->tree);
  // 
  // mrb_run( mrb, mrb_proc_new(mrb, mrb->irep[mrb_generated_code]), mrb_nil_value() );
  // 
  // int lua_return = mrb_generated_code > 0 ? 0 : 1;
  // lua_pushnumber(L, lua_return);
  // 
  // mrb_close(mrb);
  
  return 1;
}

static const struct luaL_reg mrubylib[] = {
  { "run", lua_mruby_run },
  { NULL, NULL }
};

int luaopen_mruby(lua_State *L) {
  // luaL_openlib(L, "mruby", mrubylib, 0);
  luaL_register(L, "mruby", mrubylib);
  return 1;
}
