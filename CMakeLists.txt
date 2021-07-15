cmake_minimum_required(VERSION 3.17)
project(mtk LANGUAGES C)
enable_testing()

if(CMAKE_CONFIGURATION_TYPES)
    set(FS "-fcolor-diagnostics -Werror -Weverything -ffp-contract=fast")
    set(CMAKE_C_FLAGS_LTO "${FS} -O2 -flto")
    set(CMAKE_C_FLAGS_SAN "${FS} -fsanitize=address,integer,undefined -fno-sanitize-recover=all")
    set(CMAKE_C_FLAGS_X86 "${FS} -O2 -momit-leaf-frame-pointer")

    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES};lto;san;x86")
    set(CMAKE_CROSS_CONFIGS   "all")
    set(CMAKE_DEFAULT_CONFIGS "all")
endif()

foreach(foo array gfx hash vm)
    add_library   (${foo}       ${foo}.c)
    add_executable(${foo}_test  ${foo}_test.c)
    add_executable(${foo}_bench ${foo}_bench.c)

    target_link_libraries(${foo}_test  ${foo})
    target_link_libraries(${foo}_bench ${foo})

    add_test(NAME ${foo}_test COMMAND ${foo}_test)

    set_property(TARGET ${foo} ${foo}_test ${foo}_bench
                 PROPERTY OSX_ARCHITECTURES_X86 x86_64 x86_64h)
endforeach()

target_link_libraries(vm array)
