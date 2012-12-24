-- http://luarocks.org/en/Rockspec_format
package = "mruby"
version = "0.1-0"
source = {
  -- TODO: This should be using source.tag with git url
  url = "git://github.com/RyanScottLewis/lua-mruby.git",
  -- dir = 'lua-mruby-master'
}
description = {
  summary = "Use MRuby from within Lua.",
  detailed = [[
    Create MRuby interpreters to run Ruby code from within Lua and convert Ruby objects
    to Lua objects and vice-versa.
  ]],
  homepage = "https://github.com/RyanScottLewis/lua-mruby",
  license = "MIT" -- or whatever you like
}
dependencies = {
  "lua >= 5.1"
  -- If you depend on other rocks, add them here
}
build = {
  type = "command",
  build_command = "ls -alRH && rake",
  -- modules = {
  --   lua_mruby = "lib/lua-mruby.c"
  --   -- lua_mruby = {
  --   --    sources = {"lib/lua-mruby.c"},
  --   -- --    -- defines = {"MAX_DATES_PER_MEAL=50"}
  --   -- --    -- libraries = {"date"},
  --   -- --    -- incdirs = {"$(LIBDATE_INCDIR)"},
  --   -- --    libdirs = {"lib"}
  --   -- }
  -- }
}