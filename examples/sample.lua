require "mruby"

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
-- 
