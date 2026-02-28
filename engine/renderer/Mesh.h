#pragma once

#include <memory>
#include <vector>
#include <string>

namespace NRE {

/**
 * @brief 3D mesh with geometry and material data
 * 
 * Supports LOD (Level of Detail) and virtualized geometry (Nanite-style)
 */
class Mesh {
public:
    struct Vertex {
        float position[3];
        float normal[3];
        float texCoord[2];
        float tangent[3];
        float bitangent[3];
    };

    struct LODLevel {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        float distance;  // Distance threshold for this LOD
        int triangleCount;
    };

    virtual ~Mesh() = default;

    /**
     * @brief Create mesh from vertices and indices
     * @param vertices Vertex data
     * @param indices Index data
     * @return Shared pointer to mesh
     */
    static std::shared_ptr<Mesh> Create(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices
    );

    /**
     * @brief Load mesh from file (OBJ, FBX, GLTF, etc.)
     * @param path Path to mesh file
     * @return Shared pointer to mesh
     */
    static std::shared_ptr<Mesh> Load(const std::string& path);

    /**
     * @brief Generate LOD levels automatically
     * @param lodCount Number of LOD levels to generate
     */
    virtual void GenerateLODs(int lodCount = 4) = 0;

    /**
     * @brief Add manual LOD level
     * @param lod LOD level data
     */
    virtual void AddLOD(const LODLevel& lod) = 0;

    /**
     * @brief Get LOD level for given distance
     * @param distance Distance from camera
     * @return LOD level index
     */
    virtual int GetLODLevel(float distance) const = 0;

    /**
     * @brief Get triangle count
     * @return Number of triangles
     */
    virtual int GetTriangleCount() const = 0;

    /**
     * @brief Get vertex count
     * @return Number of vertices
     */
    virtual int GetVertexCount() const = 0;
};

} // namespace NRE
