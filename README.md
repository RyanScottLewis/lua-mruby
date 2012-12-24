# lua-mruby

Use [mruby][mruby] from within [lua][lua].

## Install

### LuaRocks

> NOTE: This is not implemented yet.

```sh
$ luarocks install mruby
```

### Source

> NOTE: If `rake` is not installed, you can use the `minirake` executable in `lib/mruby/minirake/`.  
>       The `Makefile` simply calls `rake`.

```sh
$ rake
```

Or:

```sh
$ make
```

This will compile a `mruby.so` file in the `pkg` directory which you can simply `require` within Lua.

## Usage

### MRuby Table

The `mruby` table contains a few helper functions just for evaluating and running MRuby code.

#### `run`

The `run` function simply runs MRuby code. It returns a `0` if it ran without any exceptions raised and `1`
if an exception was raised.

```lua
mruby.run(" puts 'Hello, World!' ") -- prints "Hello, World!"
```

#### `eval`

The `eval` function will evaluate MRuby code. The return value will converted into a Lua object.

```lua
print(
  mruby.eval(" 'Hello,' ").." World!"
) -- prints "Hello, World!"

print(
  mruby.eval(" 50 + 50 ") + 100
) -- prints 200
```

### C Functions/Types

> NOTE: This has not been implemented yet.

All MRuby C functions/types have been ported directly to Lua.  
The name of the functions/types are identical to those of the C functions/types.

```lua
mrb_interpreter = mrb_open()
mrb_code = " puts 'Hello, World!' "
mrb_parser_state = mrb_parse_string(mrb_interpreter, mrb_code, nil)
mrb_generated_code = mrb_generate_code(mrb_interpreter, mrb_parser_state.tree)
mrb_result = mrb_run( mrb_interpreter, mrb_proc_new(mrb_interpreter, mrb_interpreter.irep[mrb_generated_code]), mrb_nil_value() )
mrb_close(mrb_interpreter)
```

## Acknowlegements

[matsumoto-r][matsumoto-r] for [mruby-on-Lua][mruby-on-Lua]. This wouldn't exist, if not for him.

## Copyright

Copyright © 2012-2013 Ryan Scott Lewis <ryan@rynet.us>.

The MIT License (MIT) - See LICENSE for further details.

[mruby]: https://github.com/mruby/mruby
[lua]: https://github.com/LuaDist/lua
[matsumoto-r]: https://github.com/matsumoto-r
[mruby-on-Lua]: https://github.com/matsumoto-r/mruby-on-Lua
