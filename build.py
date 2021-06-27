#!/usr/bin/env python3

import os
import sys

targets = [
    'array',
    'gfx',
]
bench_modes = {
    '':       '-flto -DNDEBUG',
    'arm64':  '',
    'x86_64': '-arch x86_64 -arch x86_64h -momit-leaf-frame-pointer',
}
test_modes = {
    'san':    '-fsanitize=address,integer,undefined -fno-sanitize-recover=all',
}
build_modes = {
}

header = '''
builddir = out

cc = clang -fcolor-diagnostics -Weverything -Wno-poison-system-directories

rule compile
    command = $cc -Werror -std=c99 -g -Os -ffp-contract=fast -MD -MF $out.d -c $in -o $out
    depfile = $out.d
    deps    = gcc

rule link
    command = $cc $in -o $out

rule run
    command = ./$in > $out
'''

with open('build.ninja', 'w') as f:
    print(header, file=f)
    for target in targets:
        mode  = ''
        modes = {}
        p = lambda s: print(s.format(short=target,
                                     full='out/{}/{}'.format(mode,target),
                                     flags=modes[mode]), file=f)
        modes.update(bench_modes)
        for mode in modes:
            p('build {full}_bench.o: compile {short}_bench.c')
            p('    cc = $cc {flags}')
            p('build {full}_bench: link {full}.o {full}_bench.o')
            p('    cc = $cc {flags}')

        modes.update(test_modes)
        for mode in modes:
            p('build {full}_test.o: compile {short}_test.c')
            p('    cc = $cc {flags} -Wno-float-equal')
            p('build {full}_test: link {full}.o {full}_test.o')
            p('    cc = $cc {flags}')
            p('build {full}.ok: run {full}_test')

        modes.update(build_modes)
        for mode in modes:
            p('build {full}.o: compile {short}.c')
            p('    cc = $cc {flags}')

rc = os.system(' '.join(['ninja'] + sys.argv[1:]))
os.remove('build.ninja')
if rc == 0:
    os.system('git add -u')
    sys.exit(0)
sys.exit(1)
