require 'luarocks.require'
require 'mruby'

print(
  mruby.run(" puts 'Hello, World!' ") == 0
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.run('') == 0
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.run('asifbdbdsflbdslbaibidsabifabbsydifh') == 1
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.eval(" 'Hello, World!' ") == 'Hello, World!'
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.eval("123") == 123
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.eval("50 + 50") + 100 == 200
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.eval(" puts 'Hello, World!' ") == nil
)

print('-=-=-=-=-=-=-==-=-=-=-=--=-=-')

print(
  mruby.eval(" 'Hello, ' ") .. 'World!' == 'Hello, World!'
)


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
