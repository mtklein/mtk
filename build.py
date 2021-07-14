#!/usr/bin/env python3

import os
import sys

deps = {
    'array': [],
    'hash':  [],
    'gfx':   [],
    'vm':    ['array'],
}

modes = {
    '':       '-Os',
    'lto':    '-Os -flto',
    'san':    '-O0 -fsanitize=address,integer,undefined -fno-sanitize-recover=all',
    'x86_64': '-Os -arch x86_64 -arch x86_64h -momit-leaf-frame-pointer',
}

header = '''
builddir = out

cc = clang -fcolor-diagnostics -Weverything -Xclang -nostdsysteminc

rule compile
    command = $cc -Werror -std=c99 -g -ffp-contract=fast -MD -MF $out.d -c $in -o $out
    depfile = $out.d
    deps    = gcc

rule link
    command = $cc -Wl,-dead_strip $in -o $out

rule run
    command = ./$in > $out
'''

with open('build.ninja', 'w') as f:
    print(header, file=f)
    for target in deps:
        for mode in modes:
            objs = ''.join(' out/{}/{}.o'.format(mode,dep) for dep in deps[target])
            p = lambda s: print(s.format(short=target,
                                         full='out/{}/{}'.format(mode,target),
                                         flags=modes[mode]), file=f)
            p('build {full}.o: compile {short}.c')
            p('    cc = $cc {flags}')

            p('build {full}_test.o: compile {short}_test.c')
            p('    cc = $cc {flags}')
            p('build {full}_test: link {full}.o {full}_test.o' + objs)
            p('    cc = $cc {flags}')
            p('build {full}.ok: run {full}_test')

            p('build {full}_bench.o: compile {short}_bench.c')
            p('    cc = $cc {flags}')
            p('build {full}_bench: link {full}.o {full}_bench.o' + objs)
            p('    cc = $cc {flags}')


rc = os.system(' '.join(['ninja'] + sys.argv[1:]))
os.remove('build.ninja')
if rc == 0:
    os.system('git add -u')
    sys.exit(0)
sys.exit(1)
