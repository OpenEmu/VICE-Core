VICE-core
=========

OpenEmu Core plugin for VICE 3.4

## Prerequisites

In order to build VICE-core Xcode project, follow these steps to build the OpenEmu VICE dynamic library, `liboex64sc.dylib` first.

Open a terminal and change to the `deps/vice` path:

```sh
$ cd deps/vice
```

Create a build directory

```sh
$ mkdir cmake-build
```

Generate the build scripts (GNU make by default)

```sh
cmake -B cmake-build -DUSE_OEHEADLESS=YES -DUSE_ALT_CPU=YES
```

Finally, execute a `RELEASE` build

```sh
$ cmake --build cmake-build --target oex64sc --config Release -- -j`nproc --ignore=2`
```

**NOTE**

``-- -j`nproc --ignore=2` `` instructs `make` to use all but two of your system's cores, which reduces build time considerably.