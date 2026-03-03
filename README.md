# 4JLibs

A project that aims at rebuilding the 4J libraries source code via decompilation for the Minecraft: Legacy Console Edition

## Why?

This would allow compiling the Minecraft: Legacy Console Edition source code with a modern CMake-based toolchain, expand the Renderer code, add new Input support, etc...

This would also help document the structure of their projects for decompilation projects of newer versions of this version of the game

## How can I build this?

Configure and build with CMake on Windows x64 using the same Visual Studio generator style as `MinecraftConsoles`:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_GENERATOR_INSTANCE="C:/Program Files/Microsoft Visual Studio/2022/Community"
cmake --build build --config Debug
cmake --install build --config Debug --prefix package
```

Installed output layout:

- `package/include/4JLibs`: public headers you can copy into another project
- `package/lib/Debug` or `package/lib/Release`: static libraries

## What is implemented?

All projects can be linked against the main game code, whilst there's some unnamed stuff in the Renderer, this works just fine and the game can be played
