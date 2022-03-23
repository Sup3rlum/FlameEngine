# FlameEngine
Flame is an open source game engine that I wrote my self as a way to improve knowledge of game development.

## Images

Screenshots from various scenes and editors:

<img src="https://cdn.discordapp.com/attachments/717866375212826705/947628214795255818/unknown.png" width="900" height="500">
<img src="https://cdn.discordapp.com/attachments/761699926211952700/913991686689005668/Screenshot_29.png" width="900" height="492">
<img src="https://cdn.discordapp.com/attachments/649214971590213653/904501793885679656/Screenshot_2.png" width="900" height="502">
<img src="https://cdn.discordapp.com/attachments/761699926211952700/956147912646201354/unknown.png" width="900" height="500"

## Features

### Graphics
Flame uses a deferred renderer with a PBR Metallic/Roughness workflow. Currently, the graphical features supported by the renderer are:
  - Physics based BRDF local illumination
  - CSM with Variance filtering
  - Parallax Occlusion Mapping (POMs)
  - SMAA
  - HBAO+
  - Screen Space Reflections (Currently disabled, working on improving them performance-wise)
  - HDR Rendering with ACES filimc tonemapping curves

### Environment

There is support for dynamic atmospheric scattering, however it isn't adjusted and calibrated properly. In addition to that I will be adding lens flares and support for environment/irradiance maps once I finish working on a level/environment system.

I have also developed a very flexible particle system which I have yet to use to its full potential and experiment with.

### Scene

Flame's game scene architecture is entirely based on the Entity Component System (ECS) model. It uses a custom made ECS that I specifically designed with cache optimization in mind. The system was inspired by Unity's DOTS, where entity data is tightly packed in memory buckets which are smartly allocated on the fly. Each bucket is created against an entity archetype and is represented by a chain of memory block nodes, each with the same signature. In addition, I have employed various compile time and RTTI techniques to keep the memory footprint of the system minimal, as well as to increase iteration and look up speeds of entity systems as much as possible.  

### Physics

Flame uses NVIDIA's PhysX API to simulate a physical environment. Currently only basic geometry, cooked triangle meshes and character controllers are supported. Much work is needed to integrate all of PhysX' features into flame. One of the more low-level challenges is to incorporate the API into the ECS by making use of custom heap allocators, which might actually be quite the challenge itself.

### Assets

Flame currently uses its own asset system and formats for models, materials and shaders. It is quite primitive and requires some dubios setup but works fine for now. I am planning on integrating the asset encoder and compilers in the scene editor for better productivity. (Currently I have to console-compile every asset :( ). There is also a Blender plugin that I have made which exports geometry directly into Flame's native model format FL3D.

### Editor

Currently the editor is broken. Otherwise it worked just fine, it is a WPF .NET 5 based application with hooks to the engine and a native rendering surface. Currently Interop between the engine and the editor is quite cumbersome to incorporate without a dedicated interface layer which I will work on in the future.

### Planned features

Currently the roadmap outlines itself as follows:

- Adding translucent object forward pass during rendering
- Fixing SSR
- Improving compute integration
- Improving atmospheric scattering shaders
- Introducing robust environment/level management
- Incorporating asset compilers into editor
- Integrating more of PhysX's features

## Dependencies

  - PhysX 4.1
  - HBAO+  4.0
  - DirectX 11
  - OpenGL 4.0
  - glew32
  - libpng
  - OpenSSL
  - C# 8
  - .NET Core 3.1
  - ImageSharp

## Licensing

Please contact me for any licensing enquiries.
