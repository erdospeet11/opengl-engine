# VoidEngine

A modern OpenGL-based game engine written in C++.

## Project Structure

- `src/` - Core engine source code
  - `window/` - Window management system
- `examples/` - Example projects and demos
  - `basic_window/` - Basic window creation demo
- `.github/workflows/` - CI/CD configuration files

## Features

- Window creation and management
- OpenGL context initialization
- More features coming soon!

## Dependencies

- CMake (>= 3.15)
- OpenGL
- GLFW3
- C++17 compatible compiler

## Building

### Linux

1. Install dependencies:
```bash
# Ubuntu/Debian
sudo apt-get install cmake libglfw3-dev
```

2. Build the project:
```bash
./build.sh
```

The build artifacts will be in the `build` directory.

### Windows

1. Install dependencies using vcpkg:
```bash
vcpkg install glfw3
```

2. Configure and build using CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running Examples

To run the basic window example:

```bash
./build/examples/basic_window
```

## License

MIT License 