#! /bin/sh

set -x
set -e

WASI_SDK=/opt/wasi-sdk-24.0
SYSROOT=${WASI_SDK}/share/wasi-sysroot
CC=${WASI_SDK}/bin/clang
CFLAGS="-Os -Wall -Werror -v"
TOYWASM=${TOYWASM:-toywasm}

TARGET=wasm32-wasi

${CC} ${CFLAGS} \
-L../sysroot/lib/${TARGET} -lmalloc \
-o test-static test.c

${TOYWASM} --wasi test-static

${CC} ${CFLAGS} \
-fPIC \
-Xlinker -pie \
-Xlinker --export-if-defined=__main_argc_argv \
-Xlinker --import-memory \
-Xlinker --export-memory \
-L../sysroot/lib/${TARGET} -lmalloc \
-o test test.c

${TOYWASM} --wasi --print-stats --dyld \
--dyld-path ../sysroot/lib/${TARGET} \
--dyld-path ${WASI_SDK}/share/wasi-sysroot/lib/${TARGET} \
test
