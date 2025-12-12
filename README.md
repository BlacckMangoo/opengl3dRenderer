# 3D Game

A 3D rendering engine built with OpenGL, featuring realistic PBR-like rendering with advanced lighting and texture mapping.

## Features

- **Modern OpenGL 4.3** rendering pipeline
- **PBR-like rendering** with multiple texture maps:
  - Diffuse (albedo)
  - Normal mapping for surface detail
  - Specular maps
  - Roughness maps
  - Ambient Occlusion (AO)
- **Advanced lighting**:
  - Phong lighting model
  - Tangent-space normal mapping
  - Roughness-based specular highlights
- **Camera system**:
  - WASD movement
  - Mouse rotation (left click + drag)
  - Mouse panning (middle click + drag)
  - Scroll to zoom
- **Model loading** via Assimp (OBJ format)
- **Post-processing**:
  - Tone mapping
  - Gamma correction

## Technologies

- **C++20**
- **OpenGL 4.3**
- **GLFW** - Window and input management
- **GLM** - Mathematics library
- **Assimp** - 3D model loading
- **STB Image** - Texture loading
- **GLAD** - OpenGL function loader

## Build Instructions

### Prerequisites
- CMake 3.10+
- C++20 compatible compiler
- Git

### Building

```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
cmake --build .
```

### Running

```bash
./WaterAdventure
```

## Controls

- **W/A/S/D** - Move camera
- **Left Mouse + Drag** - Rotate camera
- **Middle Mouse + Drag** - Pan camera
- **Scroll Wheel** - Zoom (adjust FOV)
- **ESC** - Exit application

## Project Structure

```
├── Assets/              # Shaders and 3D models
│   ├── lit.vert        # Lighting vertex shader
│   ├── lit.frag        # PBR fragment shader
│   └── backpack/       # Sample 3D model with textures
├── Deps/               # Dependencies (GLAD)
├── include/            # Header files
│   ├── Core/          # Camera and scene management
│   └── Renderer/      # Rendering system
├── src/               # Source files
└── CMakeLists.txt     # Build configuration
```

## License

MIT License

## Author

Built with ❤️ using modern OpenGL techniques

