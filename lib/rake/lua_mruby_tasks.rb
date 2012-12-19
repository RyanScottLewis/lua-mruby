require 'rake/tasklib'
require 'pathname'
Pathname.send(:alias_method, :/, :join)

class Rake::LuaMRubyTasks < Rake::TaskLib
  
  def initialize
    
    desc "Compile lua-mruby"
    # task(:compile, &:compile)
    task(:compile) { compile }
    
    desc "Clean up the current build"
    # task(:clean, &:clean)
    task(:clean) { clean }
    
    task :default => [:clean, :compile]
    
  end
  
  define_method(:root)          { (Pathname.new(__FILE__) / '..' / '..' / '..').expand_path }
  define_method(:compiler)      { '/usr/bin/gcc' }
  define_method(:mruby)         { Pathname.new('/Users/ryguy/Code/C/Source/mruby') }
  define_method(:lua)           { Pathname.new('/usr/local/Cellar/lua/5.1.4') }
  define_method(:mruby_include) { mruby / 'include' }
  define_method(:mruby_src)     { mruby / 'src' }
  define_method(:lua_include)   { lua / 'include' }
  define_method(:package)        { root / 'pkg' }
  define_method(:output)        { "-o #{package / 'mruby.so'}" }
      
  def includes
    [mruby_include, mruby_src, lua_include].collect { |path| "-I#{path}" }.join(' ')
  end
      
  def libraries
    [
      root / 'lib' / 'lua-mruby.c',
      '-lm',
      mruby / 'mrblib' / 'mrblib.o',
      mruby / 'lib' / 'libmruby.a'
    ].join(' ')
  end
      
  def flags
    '-llua -O3 -g -Wall -Werror-implicit-function-declaration -shared -O'
  end
      
  def command
    [compiler, includes, flags, output, libraries].join(' ')
  end
    
  def compile
    package.mkdir
    system(command)
  end
    
  def clean
    package.rmtree rescue nil
  end
  
end