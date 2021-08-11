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

apple = 'env BENCH_SEC=0.001'
brew  = 'env BENCH_SEC=0.001 ASAN_OPTIONS=detect_leaks=1'
wasm  = 'wasmtime --env BENCH_SEC=0.001'

modes = {
    '':       (apple, '$apple -Os'),
    'lsan':   (brew,  '$brew  -fsanitize=address,integer,undefined -fno-sanitize-recover=all'),
    'lto':    (apple, '$apple -Os -flto'),
    'san':    (apple, '$apple -fsanitize=address,integer,undefined -fno-sanitize-recover=all'),
    'x86_64': (apple, '$apple -Os -arch x86_64 -arch x86_64h -momit-leaf-frame-pointer'),
    'wasm':   (wasm,  '$zigcc -Os -target wasm32-wasi -D_POSIX_SOURCE -Xclang -fnative-half-type'),
}

header = '''
builddir = out

apple = clang -fcolor-diagnostics -Weverything -Xclang -nostdsysteminc
brew  = ~/brew/opt/llvm/bin/clang -fcolor-diagnostics -Weverything -Xclang -nostdsysteminc
zigcc = zig cc -fcolor-diagnostics -Weverything

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
    for target in deps:
        for mode in modes:
            runtime, cc = modes[mode]
            objs = ''.join(' out/{}/{}.o'.format(mode,dep) for dep in deps[target])
            p = lambda s: print(s.format(target  = target,
                                         full    = 'out/{}/{}'.format(mode,target),
                                         cc      = cc,
                                         runtime = runtime), file=f)
            p('build {full}.o: compile {target}.c')
            p('    cc      = {cc}')
            p('build {full}_test.o: compile {target}_test.c')
            p('    cc      = {cc}')
            p('build {full}_test: link {full}.o {full}_test.o' + objs)
            p('    cc      = {cc}')
            p('build {full}_test.ok: run {full}_test')
            p('    runtime = {runtime}')

rc = os.system(' '.join(['ninja'] + sys.argv[1:]))
os.remove('build.ninja')
if rc == 0:
    os.system('git add -u')
    sys.exit(0)
sys.exit(1)
