#pragma once

#include <memory>
#include <vector>
#include <string>

namespace NRE {

/**
 * @brief Photorealistic tree rendering and simulation
 * 
 * Procedural tree generation using L-systems, wind physics, and seasonal changes
 */
class TreeRenderer {
public:
    struct TreeConfig {
        // Generation
        std::string species = "oak";    // oak, pine, maple, birch, etc.
        int seed = 0;
        float age = 10.0f;              // Years old
        float height = 10.0f;           // Meters
        
        // L-System parameters
        int iterations = 5;
        float branchAngle = 25.0f;
        float branchLength = 1.0f;
        float branchThickness = 0.1f;
        
        // Visual detail
        int leafCount = 10000;
        bool individualLeaves = true;   // vs. billboard leaves
        int barkTextureResolution = 4096;
        
        // Physics
        bool enableWindPhysics = true;
        bool enableGrowthSimulation = false;
        bool enableSeasonalChanges = true;
    };

    enum class Season {
        Spring,     // New leaves, flowers
        Summer,     // Full foliage
        Autumn,     // Color change, falling leaves
        Winter      // Bare branches (deciduous)
    };

    virtual ~TreeRenderer() = default;

    /**
     * @brief Create tree renderer
     * @param config Tree configuration
     * @return Unique pointer to tree renderer
     */
    static std::unique_ptr<TreeRenderer> Create(const TreeConfig& config = TreeConfig{});

    /**
     * @brief Generate tree using L-system
     */
    virtual void Generate() = 0;

    /**
     * @brief Update tree (wind, growth, etc.)
     * @param deltaTime Time since last update
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Render tree
     */
    virtual void Render() = 0;

    /**
     * @brief Set current season
     * @param season Season to set
     */
    virtual void SetSeason(Season season) = 0;

    /**
     * @brief Apply wind force
     * @param direction Wind direction (normalized)
     * @param strength Wind strength (0-1)
     */
    virtual void ApplyWind(const float direction[3], float strength) = 0;

    /**
     * @brief Simulate growth over time
     * @param years Years to grow
     */
    virtual void Grow(float years) = 0;

    /**
     * @brief Get tree bounds
     * @param minBounds Output minimum bounds
     * @param maxBounds Output maximum bounds
     */
    virtual void GetBounds(float minBounds[3], float maxBounds[3]) const = 0;
};

} // namespace NRE
