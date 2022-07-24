# Vk2D : Small 2D engine written in C99

## Current features

- 2D Renderer (Vulkan backend)  
- Input  
- Window system  
- File loading  
- Math library  
- Containers (lists and hash maps)  
- Event system  
- Logger 

## How to build

```batch
git clone https://github.com/Sausty/Vk2D
git submodule init --recursive
cd example_app/vk2d_shaders
glslc vertex.vert -o vertex.spv
glslc fragment.frag -o fragment.spv
cmake -G "The generator you want" -B build
```

## Contributing

I will happily accept any contributions, mostly if it concerns the audio system i'm having so much trouble to implement.
