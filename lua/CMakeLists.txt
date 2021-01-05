cmake_minimum_required(VERSION 3.8)
project(lua)

add_library(lua STATIC
    lapi.c
    lauxlib.c
    lbaselib.c
    lcode.c
    lctype.c
    ldebug.c
    ldo.c
    ldump.c
    lfunc.c
    lgc.c
    linit.c
    llex.c
    lmem.c
    lobject.c
    lopcodes.c
    lparser.c
    lstate.c
    lstring.c
    ltable.c
    ltm.c
    lundump.c
    lvm.c
    lzio.c
    #lcorolib.c
    lmathlib.c
    lstrlib.c
    ltablib.c
    lutf8lib.c
    loadlib.c
    ldblib.c
    #liolib.c
    #loslib.c
    wrap/stdio.cpp
)

target_include_directories(lua SYSTEM PUBLIC wrap)
target_include_directories(lua PRIVATE ${32BLIT_PATH}/32blit)