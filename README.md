# lua-mruby

Use [mruby][mruby] from within [lua][lua].

## Install

```shell
$ make
```

## Usage

```lua
require "mruby"

mruby.run([[

  class User
    
    class Error < StandardError; end
    
    class InvalidOptionsError < Error
      def to_s
        'The options argument must be a Hash or respond to :to_hash or :to_h'
      end
    end
    
    class InvalidNameError < Error
      def to_s
        'The :name option must not be nil and must be a String or respond to :to_s'
      end
    end
    
    class InvalidAgeError < Error
      def to_s
        'The :age option must be an Integer or respond to :to_i'
      end
    end
    
    def initialize(options)
      raise InvalidOptionsError unless options.is_a?(Hash) || options.respond_to?(:to_hash) || options.respond_to?(:to_h)
      options = options.to_hash rescue options.to_h unless options.is_a?(Hash)
      
      raise InvalidNameError unless !options[:name].nil? || options[:name].is_a?(String) || options[:name].respond_to?(:to_s)
      @name = options[:name].to_s
      
      raise InvalidAgeError unless options[:age].nil? || options[:age].is_a?(Integer) || options[:age].respond_to?(:to_i)
      @age = options[:age].to_i
    end
    
  end

]])
```

## Copyright

Copyright © 2012 Ryan Scott Lewis <ryan@rynet.us>.

The MIT License (MIT) - See LICENSE for further details.

[message_queue]: http://en.wikipedia.org/wiki/Message_queue
[queue]: http://rubydoc.info/stdlib/thread/Queue
[service]: https://github.com/RyanScottLewis/service










* Install

        make

* Example



* Test

    ```lua
    require "mruby"
     
    file = "./test.mrb"
     
    print(">> mruby file run ./test.mrb")
    mruby.run_file(file)
     
    print(">> mruby code run")
    mruby.run("puts 'mruby code run on Lua!!'")
    ```

* Run

        lua sample.lua
        lua mruby_on.lua

        >> mruby file run ./test.mrb
        mruby file run on Lua!!
        >> mruby code run
        mruby code run on Lua!!
