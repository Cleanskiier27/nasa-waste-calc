#pragma once

#include <string>
#include <memory>
#include <vector>

namespace NRE {

/**
 * @brief Material system with Physically-Based Rendering (PBR)
 * 
 * Implements Disney BRDF and GGX microfacet model
 */
class Material {
public:
    struct PBRProperties {
        // Base color (albedo)
        float albedo[3] = {1.0f, 1.0f, 1.0f};
        
        // Metallic (0 = dielectric, 1 = metal)
        float metallic = 0.0f;
        
        // Roughness (0 = smooth, 1 = rough)
        float roughness = 0.5f;
        
        // Ambient occlusion
        float ao = 1.0f;
        
        // Subsurface scattering
        float subsurface = 0.0f;
        
        // Index of refraction (for glass, water, etc.)
        float ior = 1.45f;
        
        // Transparency
        float transparency = 0.0f;
    };

    struct Textures {
        std::string albedoMap;      // Base color texture
        std::string normalMap;      // Normal mapping
        std::string roughnessMap;   // Roughness texture
        std::string metallicMap;    // Metallic texture
        std::string aoMap;          // Ambient occlusion
        std::string heightMap;      // Height/displacement
        std::string emissiveMap;    // Emissive (glowing)
    };

    virtual ~Material() = default;

    /**
     * @brief Create material
     * @param name Material name
     * @return Shared pointer to material
     */
    static std::shared_ptr<Material> Create(const std::string& name);

    /**
     * @brief Load material from file
     * @param path Path to material file
     * @return Shared pointer to material
     */
    static std::shared_ptr<Material> Load(const std::string& path);

    /**
     * @brief Set PBR properties
     * @param props PBR properties
     */
    virtual void SetProperties(const PBRProperties& props) = 0;

    /**
     * @brief Set textures
     * @param textures Texture paths
     */
    virtual void SetTextures(const Textures& textures) = 0;

    /**
     * @brief Get PBR properties
     * @return PBR properties
     */
    virtual const PBRProperties& GetProperties() const = 0;
};

} // namespace NRE
