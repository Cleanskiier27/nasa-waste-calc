# Nature Reality Engine - Getting Started Tutorial

## Installation

### Prerequisites

Before you begin, ensure you have:

- **CMake 3.20+**: Build system
- **C++20 Compiler**: GCC 11+, Clang 13+, or MSVC 2019+
- **Vulkan SDK**: For Vulkan rendering (recommended)
- **Git**: For cloning the repository

### Step 1: Clone the Repository

```bash
git clone https://github.com/Cleanskiier27/nature-reality-engine.git
cd nature-reality-engine
```

### Step 2: Install Dependencies

#### Windows
```bash
# Install Vulkan SDK from https://vulkan.lunarg.com/
# Install Visual Studio 2019 or later with C++ tools
```

#### macOS
```bash
brew install cmake vulkan-sdk
# Or install Xcode for Metal support
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake build-essential vulkan-tools libvulkan-dev
```

### Step 3: Build the Engine

```bash
mkdir build
cd build
cmake .. -DUSE_VULKAN=ON -DENABLE_RAY_TRACING=ON -DBUILD_EXAMPLES=ON
cmake --build . --config Release
```

### Step 4: Run Tests

```bash
ctest --output-on-failure
```

### Step 5: Try Examples

```bash
# Run survival game example
./bin/examples/survival_game

# Run Minecraft enhanced example (requires Minecraft.jar)
./bin/examples/minecraft_enhanced /path/to/Minecraft.jar
```

## Your First Project

Let's create a simple nature scene with photorealistic rendering.

### Project Setup

Create a new directory for your project:

```bash
mkdir my_nature_game
cd my_nature_game
```

Create `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyNatureGame)

set(CMAKE_CXX_STANDARD 20)

find_package(NatureRealityEngine REQUIRED)

add_executable(my_game main.cpp)
target_link_libraries(my_game PRIVATE NatureRealityEngine)
```

### Basic Application

Create `main.cpp`:

```cpp
#include <NatureRealityEngine/Core/Application.h>
#include <NatureRealityEngine/Nature/TreeRenderer.h>
#include <NatureRealityEngine/Nature/WaterRenderer.h>
#include <NatureRealityEngine/Nature/WeatherSystem.h>
#include <iostream>

int main() {
    std::cout << "Creating photorealistic nature scene..." << std::endl;
    
    // Create application
    NRE::Application::Config config;
    config.title = "My Nature Scene";
    config.width = 1920;
    config.height = 1080;
    config.enableRayTracing = true;
    
    auto app = NRE::Application::Create(config);
    if (!app->Initialize()) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }
    
    // Create trees
    std::cout << "Generating trees..." << std::endl;
    NRE::TreeRenderer::TreeConfig treeConfig;
    treeConfig.species = "oak";
    treeConfig.height = 15.0f;
    treeConfig.individualLeaves = true;
    
    auto tree = NRE::TreeRenderer::Create(treeConfig);
    tree->Generate();
    
    // Create water
    std::cout << "Setting up water..." << std::endl;
    NRE::WaterRenderer::Config waterConfig;
    waterConfig.enableFFTOcean = true;
    waterConfig.enableCaustics = true;
    
    auto water = NRE::WaterRenderer::Create(waterConfig);
    water->Initialize();
    
    // Create weather
    std::cout << "Initializing weather system..." << std::endl;
    auto weather = NRE::WeatherSystem::Create();
    weather->Initialize();
    weather->SetWeather(NRE::WeatherSystem::WeatherType::PartlyCloudy, 0);
    weather->SetTimeOfDay(14, 0);  // 2 PM
    
    std::cout << "Starting game loop..." << std::endl;
    
    // Main loop (conceptual)
    while (!app->ShouldClose()) {
        float deltaTime = app->GetDeltaTime();
        
        // Update
        tree->Update(deltaTime);
        water->Update(deltaTime);
        weather->Update(deltaTime);
        
        // Render would happen here
        // ...
    }
    
    app->Shutdown();
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
```

### Build and Run

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/nature-reality-engine/install
cmake --build .
./my_game
```

## Next Steps

### Adding More Features

#### 1. Add Vegetation

```cpp
#include <NatureRealityEngine/Nature/VegetationSystem.h>

NRE::VegetationSystem::Config vegConfig;
vegConfig.grassDensity = 500000;
vegConfig.grassPhysics = true;

auto vegetation = NRE::VegetationSystem::Create(vegConfig);
vegetation->Initialize();

// In game loop
vegetation->Update(deltaTime);
```

#### 2. Add Ecosystem

```cpp
#include <NatureRealityEngine/Nature/EcosystemSimulation.h>

auto ecosystem = NRE::EcosystemSimulation::Create();
ecosystem->Initialize();

// Add animals
ecosystem->AddAnimal("deer", 100, 0, 100);
ecosystem->AddAnimal("rabbit", 50, 0, 50);

// In game loop
ecosystem->Update(deltaTime);
```

#### 3. Add Spatial Audio

```cpp
#include <NatureRealityEngine/Audio/AudioEngine.h>

auto audio = NRE::AudioEngine::Create();
audio->Initialize();

audio->LoadSound("wind.wav", "wind");
audio->LoadSound("water.wav", "water");

// Play ambient sounds
audio->PlaySound3D("wind", 0, 10, 0, 0.5f, true);
audio->PlaySound3D("water", 50, 0, 50, 0.8f, true);

// In game loop
audio->Update(cameraX, cameraY, cameraZ,
              forwardX, forwardY, forwardZ,
              upX, upY, upZ);
```

### Enhancing Existing Games

Load any game and apply photorealistic enhancements:

```cpp
#include <NatureRealityEngine/Runtime/GameLoader.h>

auto loader = NRE::GameLoader::Create();
auto game = loader->LoadGame("game.exe");

NRE::GameLoader::EnhancementOptions enhancements;
enhancements.textures = NRE::GameLoader::TextureQuality::Photorealistic8K;
enhancements.lighting = NRE::GameLoader::LightingMode::RayTraced;
enhancements.water = NRE::GameLoader::WaterMode::FluidSimulation;

game->ApplyEnhancements(enhancements);
game->Run();
```

## Common Issues

### Issue: Vulkan not found

**Solution**: Install Vulkan SDK from https://vulkan.lunarg.com/

### Issue: Ray tracing not supported

**Solution**: Requires GPU with ray tracing support (NVIDIA RTX, AMD RDNA 2+)

### Issue: Low FPS

**Solutions**:
- Lower texture resolution
- Disable ray tracing
- Reduce vegetation density
- Enable DLSS/FSR

### Issue: Build fails on macOS

**Solution**: Use Metal instead of Vulkan:
```bash
cmake .. -DUSE_METAL=ON -DUSE_VULKAN=OFF
```

## Resources

- [API Reference](API_Reference.md) - Complete API documentation
- [Architecture](Architecture.md) - Engine internals
- [Contributing](Contributing.md) - How to contribute
- [Examples](../examples/) - Example projects
- [Community Forum](https://forum.networkbuster.net) - Get help

## What's Next?

Now that you have the basics:

1. **Explore Examples**: Check out the example games in `/examples/`
2. **Read API Docs**: Learn about all available features
3. **Join Community**: Connect with other developers
4. **Build Something**: Create your photorealistic game!
5. **Contribute**: Help improve the engine

Happy coding! 🌿🎮
