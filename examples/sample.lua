-- Note that you must be in the files directory for this to work (i.e. `cd examples`):
package.cpath = package.cpath .. ";../pkg/?.so"

require "mruby"

mruby.run(" puts 'Hello, World!' " )
-- print(
 
-- )


-- print(
--   mruby.run([[
--     sdlfjbnlsjdfbn
--   ]])
-- )

-- mrbState = mrbOpen()
-- print( mrbState ) -- Print out the MRuby state
-- print( mrbState:stack() ) -- Print out the MRuby state's stack
-- mrbClose()
-- 
