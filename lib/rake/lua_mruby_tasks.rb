require 'rake/tasklib'
require 'pathname'
Pathname.send(:alias_method, :/, :join)

class Rake::LuaMRubyTasks < Rake::TaskLib
  
  def initialize
    
    namespace :compile do
      desc "Compile Lua"
      task(:lua) { compile_lua }
      desc "Compile MRuby"
      task(:mruby) { compile_mruby }
      desc "Compile Lua-MRuby"
      task(:lua_mruby) { compile_lua_mruby }
    end
    desc "Compile the current build"
    task :compile => ['compile:lua', 'compile:mruby', 'compile:lua_mruby']
    
    namespace :clean do
      desc "Clean Lua"
      task(:lua) { clean_lua }
      desc "Clean MRuby"
      task(:mruby) { clean_mruby }
      desc "Clean Lua-MRuby"
      task(:lua_mruby) { clean_lua_mruby }
    end
    desc "Clean up the current build"
    task :clean => ['clean:lua', 'clean:mruby', 'clean:lua_mruby']
    
    desc "Run specs"
    task(:spec) { spec_lua_mruby }
    task :test => :spec
    
    task :default => [:clean, :compile]
    
  end
  
  define_method(:root)          { (Pathname.new(__FILE__) / '..' / '..' / '..').expand_path }
  define_method(:lib)           { root / 'lib' }
  define_method(:spec)          { root / 'spec' }
  define_method(:compiler)      { '/usr/bin/gcc' }
  define_method(:mruby)         { lib / 'mruby' }
  define_method(:lua)           { lib / 'lua' }
  define_method(:mruby_include) { mruby / 'include' }
  define_method(:mruby_src)     { mruby / 'src' }
  define_method(:lua_include)   { lua / 'include' }
  define_method(:package)       { root / 'pkg' }
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
  
  def run(command)
    puts '-=-=-=-=-=-=-=-', '', command, '', '-=-=-=-=-=-=-=-'
    system command
  end
  
  def init_git
    unless root.join('.git').exist?
      run "cd #{root}"
      run "git init"
      run "git remote add origin https://github.com/RyanScottLewis/lua-mruby.git"
      run "git pull origin master"
      run "git submodule update --init lib/mruby/"
      run "git submodule update --init lib/lua/"
    end
  end
  
  def compile_lua
    init_git
    # run "cd #{lua} && make posix" # TODO: Not 100% sure this is needed...
  end
  
  def compile_mruby
    init_git
    run "cd #{mruby} && make"
  end
  
  def compile_lua_mruby
    init_git
    package.mkpath
    run "cd #{root} && #{command}"
  end
  
  def clean_lua
    run "cd #{lua} && make clean"
  end
  
  def clean_mruby
    run "cd #{mruby} && make clean"
  end
  
  def clean_lua_mruby
    run "cd #{root} && rm -rf #{package / 'mruby.so*'}"
  end
  
  def spec_lua_mruby
    run "tsc -f #{spec / '*_spec.lua'}"
  end
  
end