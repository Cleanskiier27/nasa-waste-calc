#pragma once

#include <memory>
#include <vector>

namespace NRE {

/**
 * @brief Vegetation rendering system for grass, flowers, and plants
 * 
 * Millions of grass blades with physics, procedural generation, and LOD
 */
class VegetationSystem {
public:
    struct Config {
        // Grass
        int grassDensity = 1000000;     // Blades per square kilometer
        float grassHeight = 0.3f;       // Meters
        bool grassPhysics = true;       // Wind interaction
        bool grassShadows = true;
        
        // Flowers
        int flowerDensity = 10000;      // Flowers per square kilometer
        std::vector<std::string> flowerTypes = {"daisy", "poppy", "tulip"};
        
        // Distribution
        float slopeThreshold = 45.0f;   // Max slope for vegetation (degrees)
        float minAltitude = 0.0f;       // Meters
        float maxAltitude = 3000.0f;    // Meters
        
        // LOD
        float maxRenderDistance = 500.0f;  // Meters
        bool useLOD = true;
    };

    virtual ~VegetationSystem() = default;

    /**
     * @brief Create vegetation system
     * @param config Vegetation configuration
     * @return Unique pointer to vegetation system
     */
    static std::unique_ptr<VegetationSystem> Create(const Config& config = Config{});

    /**
     * @brief Initialize vegetation system
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Update vegetation (wind, growth)
     * @param deltaTime Time since last update
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Render vegetation
     */
    virtual void Render() = 0;

    /**
     * @brief Populate terrain with vegetation
     * @param terrainData Terrain heightmap data
     * @param width Terrain width
     * @param height Terrain height
     */
    virtual void PopulateTerrain(const float* terrainData, int width, int height) = 0;

    /**
     * @brief Apply wind to vegetation
     * @param direction Wind direction (normalized)
     * @param strength Wind strength (0-1)
     */
    virtual void ApplyWind(const float direction[3], float strength) = 0;

    /**
     * @brief Flatten grass in area (player walked through)
     * @param x Center X position
     * @param z Center Z position
     * @param radius Radius of effect
     * @param amount Amount to flatten (0-1)
     */
    virtual void FlattenArea(float x, float z, float radius, float amount) = 0;
};

} // namespace NRE
