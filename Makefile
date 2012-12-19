MRUBY_ROOT = /Users/ryguy/Code/C/Source/mruby
LUA_ROOT = /usr/local/Cellar/lua/5.1.4

INCLUDES = -I$(MRUBY_ROOT)/include -I$(MRUBY_ROOT)/src -I$(LUA_ROOT)/include
LIB = -lm $(MRUBY_ROOT)/mrblib/mrblib.o -lm $(MRUBY_ROOT)/lib/libmruby.a
CFLAGS = $(INCLUDES) -llua -O3 -g -Wall -Werror-implicit-function-declaration -shared -O

CC = gcc

all :
	mkdir pkg
	$(CC) $(CFLAGS) -o pkg/mruby.so lib/lua-mruby.c $(LIB)

clean :
	rm -rf pkg
