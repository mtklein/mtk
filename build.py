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
    command = $cc -Werror -g -Os -ffp-contract=fast -MD -MF $out.d -c $in -o $out
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
            p = lambda s: print(s.format(short=target,
                                         full='out/{}/{}'.format(mode,target),
                                         flags=modes[mode]), file=f)
            p('build {full}.o: compile {short}.c')
            p('    cc = $cc {flags}')
            p('build {full}_test.o: compile {short}_test.c')
            p('    cc = $cc {flags}')
            p('build {full}_test: link {full}.o {full}_test.o')
            p('    cc = $cc {flags}')
            p('build {full}.ok: run {full}_test')

rc = os.system(' '.join(['ninja'] + sys.argv[1:]))
os.remove('build.ninja')
if rc == 0:
    os.system('git add -u')
sys.exit(rc)
