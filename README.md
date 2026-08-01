# realistic-map-generator

# Compilation commands

```bash
cmake -B build -G Ninja
cmake -B build -G Ninja -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build
```

# How to run

```bash
./build/src/realistic_map_generator
```
# Run and build (MacOS and Linux)
```bash
 cmake --build build && ./build/src/realistic_map_generator

```

# Installation

Linux:
Raylib has several dependencies that you need in order to run it.
List of required dependencies is listed here on the official Raylib [git repository](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).
It is sufficient to install just dependencies mentioned in the "Dependencies" section.
