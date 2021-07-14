cmake_minimum_required(VERSION 3.17)
project(mtk LANGUAGES C)

add_library(array array.c)
add_library(  gfx   gfx.c)
add_library( hash  hash.c)
add_library(   vm    vm.c)

target_link_libraries(vm array)


# TODO: gfx_test overflows stack without at least -O1 (san mode).
# TODO: x86 mode also has arm64 slice that needs removing for that mode to be relevant.

set(CMAKE_EXE_LINKER_FLAGS_LTO "-Os -flto")
set(CMAKE_EXE_LINKER_FLAGS_SAN "-O0 -fsanitize=address,integer,undefined -fno-sanitize-recover=all")
set(CMAKE_EXE_LINKER_FLAGS_X86 "-Os -arch x86_64 -arch x86_64h -momit-leaf-frame-pointer")

set(COMMON_C_FLAGS "-fcolor-diagnostics -Werror -Weverything -ffp-contract=fast")
set(CMAKE_C_FLAGS_LTO "${COMMON_C_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_LTO}")
set(CMAKE_C_FLAGS_SAN "${COMMON_C_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_SAN}")
set(CMAKE_C_FLAGS_X86 "${COMMON_C_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_X86}")

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES};lto;san;x86")
    set(CMAKE_CROSS_CONFIGS   "all")
    set(CMAKE_DEFAULT_CONFIGS "all")
endif()

add_executable(array_bench array_bench.c)
add_executable(  gfx_bench   gfx_bench.c)
add_executable( hash_bench  hash_bench.c)
add_executable(   vm_bench    vm_bench.c)

target_link_libraries(array_bench array)
target_link_libraries(  gfx_bench   gfx)
target_link_libraries( hash_bench  hash)
target_link_libraries(   vm_bench    vm)


enable_testing()

add_executable(array_test array_test.c)
add_executable(  gfx_test   gfx_test.c)
add_executable( hash_test  hash_test.c)
add_executable(   vm_test    vm_test.c)

target_link_libraries(array_test array)
target_link_libraries(  gfx_test   gfx)
target_link_libraries( hash_test  hash)
target_link_libraries(   vm_test    vm)

add_test(NAME array_test COMMAND array_test)
add_test(NAME   gfx_test COMMAND   gfx_test)
add_test(NAME  hash_test COMMAND  hash_test)
add_test(NAME    vm_test COMMAND    vm_test)

