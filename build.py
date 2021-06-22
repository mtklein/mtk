#!/usr/bin/env python3

import os
import sys

targets = [
    'array',
    'gfx',
    'ns',
]
modes = {
    'arm64':  '',
    'lto':    '-flto',
    'san':    '-fsanitize=address,integer,undefined -fno-sanitize-recover=all',
    'x86_64': '-arch x86_64',
}

header = '''
builddir = out

cc = clang -fcolor-diagnostics -Weverything -Wno-poison-system-directories

rule compile
    command = $cc -Werror -g -Os -MD -MF $out.d -c $in -o $out
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
        for mode in modes:
            flags = modes[mode]
            p = lambda s: print(s.format(t=target, m=mode, f=flags), file=f)
            p('build out/{m}/{t}.o:      compile {t}.c')
            p('    cc = $cc {f}')
            p('build out/{m}/{t}_test.o: compile {t}_test.c')
            p('    cc = $cc {f}')
            p('build out/{m}/{t}_test:   link out/{m}/{t}.o out/{m}/{t}_test.o')
            p('    cc = $cc {f}')
            p('build out/{m}/{t}.ok:     run out/{m}/{t}_test')

os.system(' '.join(['ninja'] + sys.argv[1:]))
os.remove('build.ninja')
