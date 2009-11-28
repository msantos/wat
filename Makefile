
CC=gcc
# Mac OS X: use "-m64" for a 64-bit erlang
FLAGS=-m32 -O3 -fPIC -bundle -flat_namespace -undefined suppress -fno-common -Wall
# Linux
#FLAGS=-fPIC -shared
ERL_ROOT=/usr/local/lib/erlang

all:
	$(CC) $(FLAGS) -o wat.so wat.c -I $(ERL_ROOT)/usr/include/


