# lua-mruby

Use [mruby][mruby] from within [lua][lua].

## Install

```shell
$ rake
```

## Usage

### Simple

```lua
require "mruby"

mruby.run(" puts 'Hello, world!' ")
```

### Advanced

> NOTE: This has not been implemented yet.

All MRuby C functions have been ported directly to Lua.


```lua
mrb = mrbOpen()
mrbCode = " puts 'Hello, World!' "
mrbParserState = mrbParseString(mrb, mrbCode, nil)
mrbGeneratedCode = mrbGenerateCode(mrb, mrbParserState.tree)
mrbResult = mrbRun( mrb, mrbProcNew(mrb, mrb->irep[mrb_generated_code]), mrbNilValue() )
mrbClose(mrb)
```

Or use the `mruby` helper table:

```lua
mrb = mruby.new
mrb.assign("$myname", "RyanScottLewis")
mrb.run("puts $myname")

calculationResult = mrb.eval("10 + 10")
print calculationResult -- 20
```

## Copyright

Copyright © 2012 Ryan Scott Lewis <ryan@rynet.us>.

The MIT License (MIT) - See LICENSE for further details.

[mruby]: https://github.com/mruby/mruby
[lua]: https://github.com/LuaDist/lua