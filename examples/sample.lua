-- Note that you must be in the files directory for this to work (i.e. `cd examples`):
package.cpath = package.cpath .. ";../pkg/?.so"

require "mruby"

mruby.run(" puts 'Hello, World!'" )


-- mruby.run([[
-- 
-- def tak(x, y, z)
-- 
--     if y >= x
--         z
--     else
--         tak(tak(x - 1, y, z),tak(y - 1, z, x),tak(z - 1, x, y))
--     end
-- end
-- 
-- print tak(20, 12, 7)
-- 
-- ]])

-- mrbState = mrbOpen()
-- print( mrbState ) -- Print out the MRuby state
-- print( mrbState:stack() ) -- Print out the MRuby state's stack
-- mrbClose()
-- 
