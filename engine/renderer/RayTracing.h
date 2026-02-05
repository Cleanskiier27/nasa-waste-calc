#pragma once

#include <string>
#include <memory>
#include <vector>

namespace NRE {

/**
 * @brief Ray tracing pipeline for photorealistic rendering
 * 
 * Implements hardware-accelerated ray tracing using Vulkan RT extensions
 */
class RayTracing {
public:
    struct Config {
        int maxRayDepth = 8;              // Max bounces
        int samplesPerPixel = 1;          // For path tracing
        bool enableReflections = true;
        bool enableRefractions = true;
        bool enableShadows = true;
        bool enableAmbientOcclusion = true;
        bool enableGlobalIllumination = true;
    };

    virtual ~RayTracing() = default;

    /**
     * @brief Create ray tracing pipeline
     * @param config Ray tracing configuration
     * @return Unique pointer to ray tracing system
     */
    static std::unique_ptr<RayTracing> Create(const Config& config = Config{});

    /**
     * @brief Initialize ray tracing (requires RT-capable GPU)
     * @return true if successful, false if RT not supported
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Check if ray tracing is supported on current hardware
     * @return true if supported
     */
    static bool IsSupported();

    /**
     * @brief Trace rays for a frame
     */
    virtual void TraceRays() = 0;

    /**
     * @brief Build acceleration structures for scene geometry
     */
    virtual void BuildAccelerationStructures() = 0;

    /**
     * @brief Update acceleration structures (for dynamic objects)
     */
    virtual void UpdateAccelerationStructures() = 0;
};

} // namespace NRE
