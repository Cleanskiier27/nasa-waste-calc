# Nature Reality Engine - API Reference

## Core Systems

### Application

Main application class for engine initialization and game loop.

```cpp
#include <NatureRealityEngine/Core/Application.h>

// Create application
Application::Config config;
config.title = "My Game";
config.width = 1920;
config.height = 1080;
config.enableRayTracing = true;

auto app = Application::Create(config);
app->Initialize();
app->Run();
app->Shutdown();
```

**Methods:**
- `Create(config)` - Create application instance
- `Initialize()` - Initialize engine systems
- `Run()` - Start main game loop
- `Shutdown()` - Clean up and exit
- `GetDeltaTime()` - Get frame delta time
- `GetFPS()` - Get current frames per second

### Window

Cross-platform window management.

```cpp
#include <NatureRealityEngine/Core/Window.h>

Window::Properties props;
props.title = "Game Window";
props.width = 1920;
props.height = 1080;
props.fullscreen = false;

auto window = Window::Create(props);
```

### Input

Keyboard, mouse, and controller input handling.

```cpp
#include <NatureRealityEngine/Core/Input.h>

Input::Initialize();

// Check key state
if (Input::IsKeyPressed(Input::Key::W)) {
    // Move forward
}

// Get mouse position
float mouseX, mouseY;
Input::GetMousePosition(mouseX, mouseY);
```

### Time

Frame timing and delta time tracking.

```cpp
#include <NatureRealityEngine/Core/Time.h>

Time::Initialize();

// In game loop
Time::Update();
float deltaTime = Time::GetDeltaTime();
float fps = Time::GetFPS();
```

## Renderer

### Main Renderer

Photorealistic rendering pipeline.

```cpp
#include <NatureRealityEngine/Renderer/Renderer.h>

Renderer::Config config;
config.api = Renderer::API::Vulkan;
config.enableRayTracing = true;
config.enableGlobalIllumination = true;

auto renderer = Renderer::Create(config);
renderer->Initialize();

// In game loop
renderer->BeginFrame();
renderer->RenderFrame(scene, camera);
renderer->EndFrame();
```

### Ray Tracing

Hardware-accelerated ray tracing.

```cpp
#include <NatureRealityEngine/Renderer/RayTracing.h>

// Check if ray tracing is supported
if (RayTracing::IsSupported()) {
    RayTracing::Config config;
    config.maxRayDepth = 8;
    config.enableReflections = true;
    config.enableRefractions = true;
    
    auto rayTracing = RayTracing::Create(config);
    rayTracing->Initialize();
}
```

### Material

PBR material system.

```cpp
#include <NatureRealityEngine/Renderer/Material.h>

auto material = Material::Create("wood");

Material::PBRProperties props;
props.albedo[0] = 0.6f;  // R
props.albedo[1] = 0.4f;  // G
props.albedo[2] = 0.2f;  // B
props.roughness = 0.8f;
props.metallic = 0.0f;

material->SetProperties(props);

Material::Textures textures;
textures.albedoMap = "wood_albedo_8k.png";
textures.normalMap = "wood_normal_8k.png";
textures.roughnessMap = "wood_roughness_8k.png";

material->SetTextures(textures);
```

### Mesh

3D geometry with LOD support.

```cpp
#include <NatureRealityEngine/Renderer/Mesh.h>

// Load mesh from file
auto mesh = Mesh::Load("tree.fbx");

// Generate LOD levels
mesh->GenerateLODs(4);

// Get appropriate LOD for distance
float distance = 50.0f;
int lodLevel = mesh->GetLODLevel(distance);
```

## Nature Systems

### Water Renderer

Photorealistic water with fluid simulation.

```cpp
#include <NatureRealityEngine/Nature/WaterRenderer.h>

WaterRenderer::Config config;
config.enableFluidSimulation = true;
config.enableFFTOcean = true;
config.enableReflections = true;
config.enableCaustics = true;
config.waveHeight = 1.0f;
config.windSpeed = 10.0f;

auto water = WaterRenderer::Create(config);
water->Initialize();

// In update loop
water->Update(deltaTime);
water->Render();

// Add splash effect
water->AddSplash(x, y, z, force);
```

### Tree Renderer

Procedural tree generation with L-systems.

```cpp
#include <NatureRealityEngine/Nature/TreeRenderer.h>

TreeRenderer::TreeConfig config;
config.species = "oak";
config.age = 15.0f;
config.height = 12.0f;
config.individualLeaves = true;
config.enableWindPhysics = true;

auto tree = TreeRenderer::Create(config);
tree->Generate();

// Update with wind
float windDir[3] = {1.0f, 0.0f, 0.5f};
tree->ApplyWind(windDir, 0.6f);

// Change season
tree->SetSeason(TreeRenderer::Season::Autumn);

tree->Update(deltaTime);
tree->Render();
```

### Vegetation System

Millions of grass blades and flowers.

```cpp
#include <NatureRealityEngine/Nature/VegetationSystem.h>

VegetationSystem::Config config;
config.grassDensity = 1000000;  // 1 million blades per km²
config.grassHeight = 0.3f;
config.grassPhysics = true;

auto vegetation = VegetationSystem::Create(config);
vegetation->Initialize();

// Populate terrain
vegetation->PopulateTerrain(terrainData, width, height);

// Apply wind
float windDir[3] = {1.0f, 0.0f, 0.0f};
vegetation->ApplyWind(windDir, 0.5f);

// Flatten area (player walked through)
vegetation->FlattenArea(playerX, playerZ, 1.0f, 0.8f);
```

### Weather System

Atmospheric simulation with dynamic weather.

```cpp
#include <NatureRealityEngine/Nature/WeatherSystem.h>

WeatherSystem::Config config;
config.enableAtmosphericSimulation = true;
config.enableVolumetricClouds = true;
config.temperature = 20.0f;

auto weather = WeatherSystem::Create(config);
weather->Initialize();

// Set weather
weather->SetWeather(WeatherSystem::WeatherType::Rain, 10.0f);

// Set time and location
weather->SetTimeOfDay(16, 30);  // 4:30 PM (golden hour)
weather->SetLocation(37.7749f, -122.4194f);  // San Francisco
weather->SetDate(2026, 6, 15);

// Trigger lightning
weather->TriggerLightning(x, y, z);

weather->Update(deltaTime);
weather->Render();
```

### Ecosystem Simulation

Living plants and animals with AI.

```cpp
#include <NatureRealityEngine/Nature/EcosystemSimulation.h>

EcosystemSimulation::Config config;
config.enablePlantGrowth = true;
config.enableAnimalBehavior = true;
config.enablePredatorPreyDynamics = true;

auto ecosystem = EcosystemSimulation::Create(config);
ecosystem->Initialize();

// Add plants
for (int i = 0; i < 1000; i++) {
    ecosystem->AddPlant("grass", x, y, z);
}

// Add animals
ecosystem->AddAnimal("deer", 100, 0, 100);
ecosystem->AddAnimal("wolf", 200, 0, 200);
ecosystem->AddAnimal("rabbit", 150, 0, 150);

// Update simulation
ecosystem->Update(deltaTime);

// Query populations
int deerCount = ecosystem->GetPopulation("deer");
```

## Universal Game Runtime

### Game Loader

Load and enhance any game from any platform.

```cpp
#include <NatureRealityEngine/Runtime/GameLoader.h>

auto loader = GameLoader::Create();

// Load game (auto-detect platform)
auto game = loader->LoadGame("Minecraft.jar", GameLoader::Platform::AutoDetect);

// Configure enhancements
GameLoader::EnhancementOptions enhancements;
enhancements.textures = GameLoader::TextureQuality::Photorealistic8K;
enhancements.lighting = GameLoader::LightingMode::RayTraced;
enhancements.water = GameLoader::WaterMode::FluidSimulation;
enhancements.vegetation = GameLoader::VegetationMode::ProceduralMillions;
enhancements.wildlife = GameLoader::WildlifeMode::RealisticAI;

game->ApplyEnhancements(enhancements);
game->Run();
```

**Supported Platforms:**
- PC: Windows (DirectX), macOS (Metal), Linux (Vulkan)
- Consoles: PS5, Xbox Series X/S, Switch
- Mobile: iOS, Android
- Retro: NES, SNES, N64, PS1, PS2, GameCube
- VR: Quest, PSVR2, Index, Vision Pro
- Web: HTML5, WebGL, Flash

## AI Systems

### Behavior Tree

AI decision making with behavior trees.

```cpp
#include <NatureRealityEngine/AI/BehaviorTree.h>

using namespace NRE;

// Create root selector (OR logic)
auto root = std::make_shared<BehaviorTree::Selector>();

// Add flee sequence
auto fleeSequence = std::make_shared<BehaviorTree::Sequence>();
fleeSequence->AddChild(std::make_shared<BehaviorTree::Condition>(
    []() { return IsPredatorNearby(); }
));
fleeSequence->AddChild(std::make_shared<BehaviorTree::Action>(
    []() { Flee(); return BehaviorTree::Status::Success; }
));
root->AddChild(fleeSequence);

// Add eat sequence
auto eatSequence = std::make_shared<BehaviorTree::Sequence>();
eatSequence->AddChild(std::make_shared<BehaviorTree::Condition>(
    []() { return IsHungry(); }
));
eatSequence->AddChild(std::make_shared<BehaviorTree::Action>(
    []() { FindFood(); return BehaviorTree::Status::Success; }
));
root->AddChild(eatSequence);

// Create tree
auto tree = BehaviorTree::Create(root);

// Execute each frame
BehaviorTree::Status status = tree->Execute();
```

### Pathfinding

A* pathfinding on navigation mesh.

```cpp
#include <NatureRealityEngine/AI/Pathfinding.h>

auto pathfinding = Pathfinding::Create();

// Build nav mesh from terrain
pathfinding->BuildNavMesh(terrainData, width, height, 1.0f);

// Mark obstacles as non-walkable
pathfinding->SetNonWalkable(buildingX, buildingZ, radius);

// Find path
auto path = pathfinding->FindPath(
    startX, startY, startZ,
    goalX, goalY, goalZ
);

if (path.found) {
    // Follow path
    for (size_t i = 0; i < path.positions.size(); i += 3) {
        float x = path.positions[i];
        float y = path.positions[i + 1];
        float z = path.positions[i + 2];
        // Move to position
    }
}
```

## Audio Engine

### Spatial Audio

3D audio with HRTF and environmental acoustics.

```cpp
#include <NatureRealityEngine/Audio/AudioEngine.h>

auto audio = AudioEngine::Create();
audio->Initialize();

// Load sounds
audio->LoadSound("water.wav", "water");
audio->LoadSound("bird.wav", "bird");
audio->LoadSound("footstep.wav", "footstep");

// Play 3D sound
int soundId = audio->PlaySound3D("bird", x, y, z, 0.8f, true);

// Update listener position (camera)
audio->Update(
    cameraX, cameraY, cameraZ,
    forwardX, forwardY, forwardZ,
    upX, upY, upZ
);

// Update sound position (for moving sources)
audio->UpdateSoundPosition(soundId, newX, newY, newZ);

// Set reverb for environment
AudioEngine::ReverbSettings reverb;
reverb.reverbTime = 2.0f;  // 2 seconds
reverb.roomSize = 100.0f;  // Large space
reverb.damping = 0.5f;
reverb.wetMix = 0.3f;
audio->SetReverb(reverb);
```

## Performance Tips

1. **Use LOD System**: Automatically reduce detail at distance
2. **Enable Occlusion Culling**: Don't render hidden objects
3. **Use Instancing**: Reuse geometry for vegetation
4. **Async Compute**: Overlap rendering and simulation
5. **Stream Assets**: Load on demand, don't load everything
6. **Profile**: Use built-in profiler to find bottlenecks

## Platform-Specific Notes

### Windows
- DirectX 12 recommended for best performance
- Vulkan supported via native drivers

### macOS
- Metal recommended (native API)
- Vulkan via MoltenVK

### Linux
- Vulkan recommended
- Excellent open-source driver support

### Mobile
- Use lower LOD settings
- Disable ray tracing
- Reduce texture resolution

## See Also

- [Tutorial](Tutorial.md) - Getting started guide
- [Architecture](Architecture.md) - Engine architecture
- [Contributing](Contributing.md) - How to contribute
