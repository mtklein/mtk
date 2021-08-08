#!/usr/bin/env python3

import os
import sys

deps = {
    'array':    [],
    'asm':      [],
    'checksum': [],
    'hash':     [],
    'gfx':      [],
    'vm':       ['array', 'checksum', 'hash'],
}

native_modes = {
    '':       '$clang -Os',
    'lto':    '$clang -Os -flto',
    'san':    '$clang -O0 -fsanitize=address,integer,undefined -fno-sanitize-recover=all',
    'x86_64': '$clang -Os -arch x86_64 -arch x86_64h -momit-leaf-frame-pointer',
}

wasm_modes = {
    'wasm':   '$zigcc -Os -target wasm32-wasi -D_POSIX_SOURCE -Xclang -fnative-half-type',
}

header = '''
builddir = out

clang = clang -fcolor-diagnostics -Weverything -Xclang -nostdsysteminc
zigcc = zig cc -fcolor-diagnostics -Weverything

runtime_native = env BENCH_SEC=0.001
runtime_wasm   = wasmtime --env BENCH_SEC=0.001

rule compile
    command = $cc -Werror -std=c11 -g -ffp-contract=fast -MD -MF $out.d -c $in -o $out
    depfile = $out.d
    deps    = gcc

rule link
    command = $cc $in -o $out && touch $out

rule run
    command = $runtime ./$in > $out
'''

with open('build.ninja', 'w') as f:
    print(header, file=f)
    modes = native_modes | wasm_modes
    for target in deps:
        for mode in modes:
            arch = 'native' if mode in native_modes else 'wasm'
            objs = ''.join(' out/{}/{}.o'.format(mode,dep) for dep in deps[target])
            p = lambda s: print(s.format(short = target,
                                         full  = 'out/{}/{}'.format(mode,target),
                                         cc    = modes[mode],
                                         arch  = arch), file=f)
            p('build {full}.o: compile {short}.c')
            p('    cc      = {cc}')
            p('build {full}_test.o: compile {short}_test.c')
            p('    cc      = {cc}')
            p('build {full}_test: link {full}.o {full}_test.o' + objs)
            p('    cc      = {cc}')
            p('build {full}_test.ok: run {full}_test')
            p('    runtime = $runtime_{arch}')


rc = os.system(' '.join(['ninja'] + sys.argv[1:]))
os.remove('build.ninja')
if rc == 0:
    os.system('git add -u')
    sys.exit(0)
sys.exit(1)
