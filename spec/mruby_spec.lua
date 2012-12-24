require "luarocks.require"
require "telescope" -- https://github.com/norman/telescope
require "lfs"

package.cpath = package.cpath .. ";"..lfs.currentdir().."/pkg/mruby.so"
require "mruby"

context("mruby", function()
  
  context(".run", function()
    
    test("When valid code is given it should return 0", function()
      assert_equal( mruby.run(""), 0)
      assert_equal( mruby.run(" 10 + 10 "), 0)
      assert_equal( mruby.run(" puts 'Hello, World!' "), 0)
    end)
    
    test("When invalid code is given it should return 1", function()
      assert_equal( mruby.run(" foo "), 1)
      assert_equal( mruby.run(" 10 + 'hello' "), 1)
    end)
    
  end)
  
  context(".eval", function()
    
    test("When valid code is given it should return the MRuby object converted into a Lua object", function()
      assert_equal( mruby.eval(" 'Hello, World!' "), "Hello, World!")
      assert_equal( mruby.eval(" 123 "), 123)
      assert_equal( mruby.eval(" 50 + 50 ") + 100, 200)
      assert_equal( mruby.eval(" puts 'Hello, World!' "), nil)
      assert_equal( mruby.eval(" 'Hello, ' ").."World!", "Hello, World!")
    end)
    
    -- FIXME: in 0.1-1 : Should have more elegant way of capturing errors...
    test("When invalid code is given it should return exception as a string", function()
      assert_equal( mruby.eval(" foo "), "NoMethodError: undefined method 'foo' for main")
      assert_equal( mruby.eval(" 10 + 'hello' "), "TypeError: non float value")
    end)
    
  end)
  
end)


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
