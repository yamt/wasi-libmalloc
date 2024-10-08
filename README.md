# wasi libmalloc

a stripped version of [wasi-libc], which only contains malloc.

[wasi-libc]: https://github.com/WebAssembly/wasi-libc

```shell
CC=/opt/wasi-sdk-24.0/bin/clang make libc
CC=/opt/wasi-sdk-24.0/bin/clang make libc_so
CC=/opt/wasi-sdk-24.0/bin/clang make THREAD_MODEL=posix libc
```

```shell
% find sysroot
sysroot
sysroot/lib
sysroot/lib/wasm32-wasi
sysroot/lib/wasm32-wasi/libmalloc.so
sysroot/lib/wasm32-wasi/libmalloc.a
sysroot/lib/wasm32-wasi-threads
sysroot/lib/wasm32-wasi-threads/libmalloc.a
%
```

## tests

the `test-hook` branch has a few test code.

## CAVEATS

the shared library doesn't work well because:

* https://github.com/llvm/llvm-project/issues/104575

* a few things like duplocale use the libc-internal hidden aliases.
  maybe we should stop using the __heap_base/__heap_end region.
