# C-Renderer
A 3D Graphics renderer written in C alongside Pikumas "Fundamentals of 3D Graphics" course. This repository was an academic exercise in learning how 3D graphics work and not meant to be representative of production level code. This repository specifically is configured to compile on linux/ubuntu based systems. 

Currently the 3D renderer supports:
- Scanline triangle rasterization.
- Rendering of multiple meshes/texture via .obj files and .png files.
- Full software graphics pipeline
- Flat shading lighting model
///////////////////////////////////////////////////////////////////////////////
// Process the graphics pipeline stages for all the mesh triangles
///////////////////////////////////////////////////////////////////////////////
// +-------------+
// | Model space |  <-- original mesh vertices
// +-------------+
// |   +-------------+
// -> | World space |  <-- multiply by world matrix
//     +-------------+
//     |   +--------------+
//     -> | Camera space |  <-- multiply by view matrix
//         +--------------+
//         |    +------------+
//         --> |  Clipping  |  <-- clip against the six frustum planes
//              +------------+
//              |    +------------+
//              --> | Projection |  <-- multiply by projection matrix
//                   +------------+
//                   |    +-------------+
//                   --> | Image space |  <-- apply perspective divide
//                        +-------------+
//                        |    +--------------+
//                        --> | Screen space |  <-- ready to render
//                             +--------------+
///////////////////////////////////////////////////////////////////////////////

In order to compile you will need to install the linux build essentials and the SDL development libraries via your favorite package manager.
If you're using apt run the following commands:
- `sudo apt install build-essential`
- `sudo apt install libsdl2-dev`

- 

![image](https://github.com/BeyondBelief96/C-Renderer/assets/50305220/067ca2aa-2ee2-4f7b-92b8-de82e6fdeebc)


