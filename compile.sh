#!/bin/sh
cd "`dirname $0`"
cc -Wl,-s,-x,--hash-style=sysv -O3 -fPIE -pie -std=gnu90 -Wall -Wextra -pedantic -o bin/gauss src/*.c
