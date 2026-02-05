# Contributing to Nature Reality Engine

Thank you for your interest in contributing! This guide will help you get started.

## Code of Conduct

Be respectful, inclusive, and collaborative. We welcome contributors of all skill levels.

## Getting Started

### 1. Fork the Repository

Click the "Fork" button on GitHub to create your own copy.

### 2. Clone Your Fork

```bash
git clone https://github.com/YOUR_USERNAME/nature-reality-engine.git
cd nature-reality-engine
```

### 3. Create a Branch

```bash
git checkout -b feature/my-awesome-feature
```

## Development Setup

### Prerequisites

- CMake 3.20+
- C++20 compiler (GCC 11+, Clang 13+, MSVC 2019+)
- Vulkan SDK (or Metal/DirectX for other platforms)
- Git

### Build for Development

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build .
```

### Run Tests

```bash
ctest --output-on-failure
```

## Coding Standards

### C++ Style Guide

**Naming Conventions:**
```cpp
// Classes: PascalCase
class TreeRenderer { };

// Functions: PascalCase
void RenderFrame();

// Variables: camelCase
int frameCount;

// Constants: UPPER_SNAKE_CASE
const int MAX_LIGHTS = 256;

// Members: m_ prefix
class Example {
    int m_Count;
    float m_DeltaTime;
};

// Namespaces: PascalCase
namespace NRE { }
```

**File Organization:**
```
Header files: .h
Implementation files: .cpp
Headers in engine/module_name/
Implementations in src/module_name/ (future)
```

**Comments:**
```cpp
/**
 * @brief Short description
 * 
 * Longer description explaining the purpose,
 * behavior, and any important details.
 * 
 * @param name Parameter description
 * @return Return value description
 */
void Function(int name);
```

### Code Quality

**Modern C++ Practices:**
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Use RAII for resource management
- Prefer `auto` when type is obvious
- Use `constexpr` for compile-time constants
- Use range-based for loops when appropriate

**Example:**
```cpp
// Good
auto mesh = Mesh::Create(vertices, indices);
for (const auto& vertex : vertices) {
    // Process vertex
}

// Avoid
Mesh* mesh = new Mesh(vertices, indices);  // Manual memory management
for (size_t i = 0; i < vertices.size(); i++) {
    // Index-based loop
}
```

## Areas for Contribution

### High Priority

1. **Core Renderer Implementation**
   - Vulkan backend implementation
   - Ray tracing pipeline
   - Deferred rendering
   - LOD system

2. **Nature Systems**
   - Tree generation (L-systems)
   - Water simulation (FFT ocean, Navier-Stokes)
   - Vegetation rendering (grass, flowers)
   - Weather simulation

3. **Universal Runtime**
   - Platform adapters (DirectX → Vulkan, etc.)
   - Game loader
   - Enhancement pipeline

4. **Physics Engine**
   - Integrate Bullet Physics
   - Fluid simulation (SPH)
   - Soft body dynamics

### Medium Priority

5. **AI Systems**
   - Behavior tree implementation
   - A* pathfinding
   - Utility-based AI

6. **Audio Engine**
   - Spatial audio with HRTF
   - Reverb and acoustics
   - Doppler effect

7. **Developer Tools**
   - Visual editor (ImGui-based)
   - Terrain editor
   - Material editor

### Low Priority but Welcome

8. **Examples and Tutorials**
   - More example games
   - Video tutorials
   - Documentation improvements

9. **Platform Support**
   - Additional platform backends
   - Mobile optimization
   - Web support (WebGPU)

10. **Optimization**
    - Performance profiling
    - Memory optimization
    - GPU optimization

## How to Contribute

### Bug Reports

Create an issue with:
- Clear title and description
- Steps to reproduce
- Expected vs actual behavior
- System information (OS, GPU, drivers)
- Screenshots/logs if applicable

### Feature Requests

Create an issue with:
- Clear use case
- Proposed API (if applicable)
- Why it's needed
- Potential implementation approach

### Pull Requests

1. **One feature per PR**: Keep PRs focused
2. **Write tests**: Add tests for new features
3. **Update docs**: Document new APIs
4. **Follow style guide**: Consistent code style
5. **Commit messages**: Clear, descriptive messages

**PR Checklist:**
- [ ] Code follows style guide
- [ ] Tests pass (`ctest`)
- [ ] New tests added for new features
- [ ] Documentation updated
- [ ] No compiler warnings
- [ ] Builds on all platforms

**Commit Message Format:**
```
[Module] Short description (50 chars max)

Longer explanation of what changed and why.
Reference any issues: Fixes #123
```

Examples:
```
[Renderer] Add ray tracing support

Implements hardware-accelerated ray tracing using Vulkan RT extensions.
Includes reflections, refractions, and soft shadows.

Fixes #42
```

## Testing

### Unit Tests

Write tests for new features:

```cpp
// tests/test_my_feature.cpp
#include <iostream>

int main() {
    std::cout << "Testing MyFeature..." << std::endl;
    
    // Test 1
    // Test 2
    // Test 3
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
```

Add to `tests/CMakeLists.txt`:
```cmake
add_executable(test_my_feature test_my_feature.cpp)
target_link_libraries(test_my_feature PRIVATE NatureRealityEngine)
add_test(NAME MyFeatureTest COMMAND test_my_feature)
```

### Integration Tests

Test multiple systems working together.

### Performance Tests

Use profiling tools to ensure no performance regressions.

## Documentation

### API Documentation

Use Doxygen-style comments:

```cpp
/**
 * @brief Render a frame with photorealistic quality
 * 
 * This function performs deferred rendering with ray tracing,
 * global illumination, and post-processing effects.
 * 
 * @param scene Scene to render
 * @param camera Camera to render from
 * @throws std::runtime_error if rendering fails
 * 
 * @note Call BeginFrame() before this function
 * @see BeginFrame(), EndFrame()
 */
void RenderFrame(Scene* scene, Camera* camera);
```

### User Documentation

Update markdown docs in `docs/`:
- `API_Reference.md` - API documentation
- `Tutorial.md` - Getting started guide
- `Architecture.md` - Engine architecture

## Review Process

1. **Submit PR**: Push to your fork and create PR
2. **CI Checks**: Automated tests must pass
3. **Code Review**: Maintainers review code
4. **Address Feedback**: Make requested changes
5. **Approval**: PR is approved and merged

**Review Criteria:**
- Code quality and style
- Test coverage
- Documentation
- Performance impact
- Cross-platform compatibility

## Community

### Communication Channels

- **GitHub Issues**: Bug reports, features
- **GitHub Discussions**: Questions, ideas
- **Discord**: Real-time chat (coming soon)
- **Forum**: https://forum.networkbuster.net

### Getting Help

- Read the [Tutorial](Tutorial.md)
- Check [API Reference](API_Reference.md)
- Search existing issues
- Ask on GitHub Discussions
- Join community forum

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Recognition

Contributors are recognized in:
- `CONTRIBUTORS.md` file
- GitHub contributors page
- Release notes for significant contributions

## Questions?

Not sure where to start? Here are some good first issues:

- Documentation improvements
- Adding code comments
- Writing tests
- Fixing typos
- Small bug fixes

Look for issues labeled `good-first-issue` on GitHub.

## Thank You!

Every contribution makes Nature Reality Engine better. Whether you're fixing a typo or implementing a major feature, your help is appreciated!

Let's build the future of gaming together! 🚀🌿🎮
