#pragma once

#include <memory>
#include <vector>

namespace NRE {

class Scene;
class Camera;

/**
 * @brief Main renderer for Nature Reality Engine
 * 
 * Implements photorealistic rendering with ray tracing, PBR, and advanced effects
 */
class Renderer {
public:
    enum class API {
        Vulkan,
        DirectX12,
        Metal,
        OpenGL
    };

    struct Config {
        API api = API::Vulkan;
        bool enableRayTracing = true;
        bool enableGlobalIllumination = true;
        bool enableVolumetrics = true;
        int maxTextureResolution = 8192;  // 8K textures
        int shadowMapResolution = 4096;
    };

    virtual ~Renderer() = default;

    /**
     * @brief Create renderer instance
     * @param config Renderer configuration
     * @return Unique pointer to renderer
     */
    static std::unique_ptr<Renderer> Create(const Config& config = Config{});

    /**
     * @brief Initialize renderer
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Render a frame
     * @param scene Scene to render
     * @param camera Camera to render from
     */
    virtual void RenderFrame(Scene* scene, Camera* camera) = 0;

    /**
     * @brief Begin frame rendering
     */
    virtual void BeginFrame() = 0;

    /**
     * @brief End frame rendering and present
     */
    virtual void EndFrame() = 0;

    /**
     * @brief Resize viewport
     * @param width New width
     * @param height New height
     */
    virtual void OnResize(unsigned int width, unsigned int height) = 0;

    /**
     * @brief Shutdown renderer
     */
    virtual void Shutdown() = 0;
};

} // namespace NRE
