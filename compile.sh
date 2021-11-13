#!/bin/sh
cd "`dirname $0`"
cc -Wl,-sx,--hash-style=sysv -O3 -pie -o bin/gauss src/gauss.c
