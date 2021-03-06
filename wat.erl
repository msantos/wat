-module(wat).

-export([init/0,init/1,get/1,set/2,add/2]).

-on_load(on_load/0).

init() ->
    on_load().
init(N) ->
    on_load(N).

on_load() ->
    on_load(32).
on_load(N) ->
    Lib = filename:join([
            filename:dirname(code:which(?MODULE)),
            ?MODULE
        ]),
    erlang:load_nif(Lib, N).

get(_) ->
    erlang:error(not_implemented).

set(_,_) ->
    erlang:error(not_implemented).

add(_,_) ->
    erlang:error(not_implemented).


