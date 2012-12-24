#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <string.h> # TODO: Is this needed?

#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/string.h>

#ifndef ENABLE_STDIO
static void p(mrb_state *mrb, mrb_value obj) {
  obj = mrb_funcall(mrb, obj, "inspect", 0);
  fwrite(RSTRING_PTR(obj), RSTRING_LEN(obj), 1, stdout);
  putc('\n', stdout);
}
#else
#define p(mrb,obj) mrb_p(mrb,obj)
#endif

// lua_pushboolean
// lua_pushcclosure
// lua_pushcfunction
// lua_pushfstring
// lua_pushinteger
// lua_pushlightuserdata
// lua_pushliteral
// lua_pushlstring
// lua_pushnil
// lua_pushnumber
// lua_pushstring
// lua_pushthread
// lua_pushvalue
// lua_pushvfstring

// No underscores in function name because apparently the Lua developers do not believe in it O_o
void lua_pushmrbvalue(lua_State *L, mrb_state *lua_mrb_state, mrb_value object) {
  
  if( mrb_fixnum_p(object) ) {
    lua_pushinteger(L, object.value.i);
  } else if( mrb_float_p(object) ) {
    lua_pushnumber(L, object.value.f);
  } else if( mrb_undef_p(object) ) {
    // TODO
  } else if( mrb_nil_p(object) ) {
    lua_pushnil(L);
  } else if( mrb_symbol_p(object) ) {
    // TODO
  } else if( mrb_array_p(object) ) {
    // TODO
  } else if( mrb_string_p(object) ) {
    // char string_value = mrb_string_value_cstr(lua_mrb_state, object);
    char *string_value = mrb_string_value_ptr(lua_mrb_state, object);
    lua_pushstring(L, string_value);
  } else if( mrb_hash_p(object) ) {
    // TODO: Table
    // lua_newtable(L);
    // lua_pushnumber(L, obj->value1); 
    // lua_setfield(L,-2,"value1"); 
    // lua_pushstring(L,obj->str); 
    // lua_setfield(L,-2,"str"); 
  } else if( mrb_voidp_p(object) ) {
    // TODO
  } else if( mrb_type(object) == MRB_TT_TRUE ) {
    lua_pushboolean(L, 1);
  } else if( mrb_type(object) == MRB_TT_FALSE ) {
    lua_pushboolean(L, 0);
  } else {
    // TODO
  }
  
}

// FIXME: lua_mruby_run and lua_mruby_eval have lots of overlapping code
// FIXME: lua_mruby_run and lua_mruby_eval must check for only one argument
// FIXME: lua_mruby_run and lua_mruby_eval must check for only argument string validity

int lua_mrb_run(lua_State *L) {
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
    lua_pushnumber(L, 1); // Push error exit code
  } else {
    // Generate MRuby bytecode
    lua_mrb_bytecode = mrb_generate_code(lua_mrb_state, lua_mrb_parser);
    
    // Evaluate MRuby bytecode
    // TODO: Should this check to see if lua_mrb_bytecode is greater than 0?
    lua_mrb_result = mrb_run(
        lua_mrb_state,
        mrb_proc_new( lua_mrb_state, lua_mrb_state->irep[lua_mrb_bytecode] ), /* pass a proc for eval */
        mrb_top_self(lua_mrb_state)
    );
    
    if (lua_mrb_state->exc) { // Check for exception
      lua_pushnumber(L, 1);   // Push error exit code
      lua_mrb_state->exc = 0; // Reset exception flag
    } else {                  // Pass the exit code to Lua
      // Was any bytecode generated?
      // If not, there was a parser error.
      int lua_return_number;
      lua_return_number = lua_mrb_bytecode > 0 ? 0 : 1;
      lua_pushnumber(L, lua_return_number);
    }
  }
  
  mrb_parser_free(lua_mrb_parser);
  mrbc_context_free(lua_mrb_state, lua_mrb_context);
  mrb_close(lua_mrb_state);
  
  return 1; // This Lua function has 1 return value on the Lua stack
}

int lua_mrb_eval(lua_State *L) {
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
    // TODO: Something.... elegant =p
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
    
    // Check for exception
    if (lua_mrb_state->exc) {
      mrb_value lua_mrb_exception_value = mrb_obj_value(lua_mrb_state->exc);
      mrb_value lua_mrb_exception_inspect_value = mrb_funcall(lua_mrb_state, lua_mrb_exception_value, "inspect", 0);
      
      char *string_value = mrb_string_value_ptr(lua_mrb_state, lua_mrb_exception_inspect_value);
      lua_pushstring(L, string_value);
      
      lua_mrb_state->exc = 0; // Reset exception flag
    } else {
      lua_pushmrbvalue(L, lua_mrb_state, lua_mrb_result);   // Pass the MRuby result to Lua
    }
  }
  
  mrb_parser_free(lua_mrb_parser);
  mrbc_context_free(lua_mrb_state, lua_mrb_context);
  mrb_close(lua_mrb_state);
  
  return 1; // This Lua function has 1 return value on the Lua stack
}


static const struct luaL_reg mrubylib[] = {
  { "run", lua_mrb_run },
  { "eval", lua_mrb_eval },
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
