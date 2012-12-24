#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <string.h> # TODO: Is this needed?

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>

#ifndef ENABLE_STDIO
#include <mruby/string.h>
static void p(mrb_state *mrb, mrb_value obj) {
  obj = mrb_funcall(mrb, obj, "inspect", 0);
  fwrite(RSTRING_PTR(obj), RSTRING_LEN(obj), 1, stdout);
  putc('\n', stdout);
}
#else
#define p(mrb,obj) mrb_p(mrb,obj)
#endif


static int lua_mruby_run(lua_State *L) {
  mrbc_context *lua_mrb_context;
  struct mrb_parser_state *lua_mrb_parser;
  mrb_state *lua_mrb_state;
  mrb_value lua_mrb_result;
  int lua_mrb_bytecode;
  
  // Convert the Lua function's first argument into a char array
  const char *lua_mrb_code = lua_tostring(L, 1); // TODO: Is the `const` needed?
  
  // Create new MRuby interpreter
  lua_mrb_state = mrb_open();
  if (lua_mrb_state == NULL) {
    fprintf(stderr, "Invalid mrb interpreter, exiting mirb");
    return EXIT_FAILURE;
  }
  
  // Create new MRuby context
  lua_mrb_context = mrbc_context_new(lua_mrb_state);
  lua_mrb_context->capture_errors = 1;
  
  // Create new MRuby parser
  lua_mrb_parser = mrb_parser_new(lua_mrb_state);
  lua_mrb_parser->s = lua_mrb_code;
  lua_mrb_parser->send = lua_mrb_code + strlen(lua_mrb_code);
  lua_mrb_parser->lineno = 1;
  
  // Parse MRuby code string from Lua
  mrb_parser_parse(lua_mrb_parser, lua_mrb_context);
  
  if (0 < lua_mrb_parser->nerr) {
    // MRuby syntax error
    printf("line %d: %s\n", lua_mrb_parser->error_buffer[0].lineno, lua_mrb_parser->error_buffer[0].message);
  } else {
    // Generate MRuby bytecode
    lua_mrb_bytecode = mrb_generate_code(lua_mrb_state, lua_mrb_parser);
    
    // Evaluate MRuby bytecode
    lua_mrb_result = mrb_run(
        lua_mrb_state,
        mrb_proc_new( lua_mrb_state, lua_mrb_state->irep[lua_mrb_bytecode] ), /* pass a proc for eval */
        mrb_top_self(lua_mrb_state)
    );
    
    if (lua_mrb_state->exc) {   // Check for exception
      p(lua_mrb_state, mrb_obj_value(lua_mrb_state->exc));
      lua_mrb_state->exc = 0;
    } else {                    // Pass the MRuby result to Lua
      
      // TODO: Function to figure out exactly what the MRuby lua_mrb_result is and push
      //       that object to the Lua stack
      
      // int *lua_return = lua_mrb_result.value.p;
      // printf("%d\n", *lua_return);
      // int lua_return_number = *lua_return > 0 ? 0 : 1;
      // lua_pushnumber(L, lua_return_number);
      // 
      // p(lua_mrb_state, result);
    }
  }
  
  mrb_parser_free(lua_mrb_parser);
  mrbc_context_free(lua_mrb_state, lua_mrb_context);
  mrb_close(lua_mrb_state);
  
  return 0;
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
