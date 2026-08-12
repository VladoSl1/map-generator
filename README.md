### realistic-map-generator

A procedurally generated, infinite 2D world map. Terrain is built on Voronoi diagram mesh, colored based on layered noise for elevation, moisture and temperature. The world is generated in realtime based on the camera position and rendered with Raylib.

## How to run

# Dependencies

Linux:
Raylib has several dependencies that you need in order to run it.
List of required dependencies is listed here on the official Raylib [git repository](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).
It is sufficient to install just dependencies mentioned in the "Dependencies" section.

# Build

```bash
cmake -B build -G Ninja
```

For ARM-based MacOS:
```bash
cmake -B build -G Ninja -DCMAKE_OSX_ARCHITECTURES=arm64
```

# Compile and Run

MacOS and Linux:
```bash
 cmake --build build && ./build/src/realistic_map_generator
```

Windows:
```bash
 cmake --build build && .\build\src\realistic_map_generator.exe
```

## Controls

| Action | Input |
|---|---|
| Pan camera | Hold left mouse button + drag |
| Zoom | Mouse wheel |
