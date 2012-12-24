#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/compile.h"


# NOTE: lib/mruby/tools/mirb.c has almost all this functionality created by someone way better
#       at this than I am. Pull code from there into here. =)

static int lua_mruby_run(lua_State *L) {
  mrb_state *mrb_interpreter = mrb_open();
  const char *mrb_code = lua_tostring(L, 1);
  struct mrb_parser_state *mrb_parser = mrb_parser_new(mrb_interpreter);
  mrbc_context *mrb_context;
  mrb_value mrb_return_value;
    
  mrb_context = mrbc_context_new(mrb_interpreter);
  mrb_context->capture_errors = 1;
  
  // Parse the code - TODO: Convert into function
  mrb_parser->s = mrb_code;
  mrb_parser->send = mrb_code + strlen(mrb_code);
  mrb_parser->capture_errors = 1;
  mrb_parser->lineno = 1;
  
  mrb_parser_parse(mrb_parser, mrb_context);
  
  if (0 < mrb_parser->nerr) {
    printf("%d\n", mrb_parser->nerr);
    // Check for parser error - TODO: Convert into function
    char *mrb_error_message;
    size_t sz;
    sz = snprintf(NULL, 0, "line %d: %s\n", mrb_parser->error_buffer[0].lineno, mrb_parser->error_buffer[0].message);
    mrb_error_message = malloc(sz + 1); /* make sure you check for != NULL in real code */
    snprintf(mrb_error_message, sz+1, "line %d: %s\n", mrb_parser->error_buffer[0].lineno, mrb_parser->error_buffer[0].message);
    
    lua_pushstring(L, mrb_error_message);
  } else { 
    // TODO: Convert into function:
    // Generate bytecode
    int mrb_byte_code = mrb_generate_code(mrb_interpreter, mrb_parser);
    // Generate Proc from bytecode for evaulation
    struct RProc *mrb_proc = mrb_proc_new( mrb_interpreter, mrb_interpreter->irep[mrb_byte_code] );
    // Evaluate the bytecode
    mrb_return_value = mrb_run(mrb_interpreter, mrb_proc, mrb_nil_value());
    // Check for exception
    if (mrb_interpreter->exc) {
      mrb_p(mrb_interpreter, mrb_obj_value(mrb_interpreter->exc));
      mrb_interpreter->exc = 0;
    } else {
      int *lua_return = mrb_return_value.value.p;
      printf("%d\n", *lua_return);
      int lua_return_number = *lua_return > 0 ? 0 : 1;
      lua_pushnumber(L, lua_return_number);
    }
    
  }
  
  mrb_parser_free(mrb_parser);
  mrbc_context_free(mrb_interpreter, mrb_context);
  mrb_close(mrb_interpreter);
  
  return 1;
}

static const struct luaL_reg mrubylib[] = {
  { "run", lua_mruby_run },
  { NULL, NULL }
};






////////////////////////////////////////
//// EXAMPLE OF PUSHING STRUCT TO LUA //
////////////////////////////////////////
// 
// typedef struct MyStruct_s { 
//   int value1; 
//   const char * str; 
// } MyStruct_t; 
// 
// void MyStruct_toTable (MyStruct_t *obj, lua_State *L) 
// { 
//   lua_newtable(L); 
//   lua_pushnumber(L, obj->value1); 
//   lua_setfield(L,-2,"value1"); 
//   lua_pushstring(L,obj->str); 
//   lua_setfield(L,-2,"str"); 
// }








// static int lua_mrb_nil_value(lua_State *L) {
//   mrb_value value = mrb_nil_value();
//   
//   lua_pushlightuserdata(L, &value);
//   
//   return 1;
// }
// 
// static int lua_mrb_open(lua_State *L) {
//   mrb_state *mrb = mrb_open();
//   
//   
//   lua_pushlightuserdata(L, mrb);
//   
//   return 1;
// }
// 
// static int lua_mrb_close(lua_State *L) {
//   mrb_state *mrb = lua_topointer(L, 1); // Convert the first argument to a C pointer
//   
//   mrb_close(mrb);
//   
//   return 1;
// }
// 
// static const struct luaL_reg mrblib[] = {
//   { "mrbNilValue" , lua_mrb_nil_value },
//   { "mrbOpen"     , lua_mrb_open      },
//   { "mrbClose"    , lua_mrb_close     },
//   { NULL, NULL }
// };





int luaopen_mruby(lua_State *L) {
  // luaL_register(L, NULL, mrblib);
  // lua_register(L, "mrbOpen", lua_mrb_open);
  // lua_register(L, "mrbClose", lua_mrb_close);
  
  luaL_register(L, "mruby", mrubylib);
  return 1;
}
