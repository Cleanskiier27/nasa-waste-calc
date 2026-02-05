#pragma once

#include <memory>
#include <vector>

namespace NRE {

/**
 * @brief Photorealistic water rendering with fluid simulation
 * 
 * Implements Navier-Stokes fluid dynamics, FFT ocean waves, reflections, refractions, and caustics
 */
class WaterRenderer {
public:
    struct Config {
        // Simulation
        bool enableFluidSimulation = true;  // Navier-Stokes
        bool enableFFTOcean = true;         // FFT-based ocean waves
        int simulationResolution = 512;
        
        // Rendering
        bool enableReflections = true;
        bool enableRefractions = true;
        bool enableCaustics = true;
        bool enableFoam = true;
        bool enableSplashes = true;
        
        // Physical properties
        float waveHeight = 1.0f;
        float waveSpeed = 1.0f;
        float windSpeed = 10.0f;
        float windDirection[2] = {1.0f, 0.0f};
        
        // Visual properties
        float clarity = 0.8f;           // 0 = murky, 1 = crystal clear
        float color[3] = {0.0f, 0.3f, 0.5f};  // Water color
        float ior = 1.333f;             // Index of refraction
    };

    virtual ~WaterRenderer() = default;

    /**
     * @brief Create water renderer
     * @param config Water configuration
     * @return Unique pointer to water renderer
     */
    static std::unique_ptr<WaterRenderer> Create(const Config& config = Config{});

    /**
     * @brief Initialize water system
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Update water simulation
     * @param deltaTime Time since last update
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Render water
     */
    virtual void Render() = 0;

    /**
     * @brief Add splash effect at position
     * @param x X position
     * @param y Y position (water surface)
     * @param z Z position
     * @param force Splash strength
     */
    virtual void AddSplash(float x, float y, float z, float force) = 0;

    /**
     * @brief Get water height at position
     * @param x X position
     * @param z Z position
     * @return Water height (Y coordinate)
     */
    virtual float GetWaterHeight(float x, float z) const = 0;
};

} // namespace NRE
