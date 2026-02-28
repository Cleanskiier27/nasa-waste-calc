#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace NRE {

/**
 * @brief A* pathfinding for navigation
 * 
 * Finds optimal path between two points on a navigation mesh
 */
class Pathfinding {
public:
    struct Node {
        float x, y, z;              // Position
        float g;                    // Cost from start
        float h;                    // Heuristic to goal
        float f;                    // Total cost (g + h)
        Node* parent;               // Previous node in path
        bool walkable;              // Can walk on this node
    };

    struct Path {
        std::vector<float> positions;  // Flat array: [x1,y1,z1, x2,y2,z2, ...]
        float totalDistance;
        bool found;
    };

    /**
     * @brief Create pathfinding system
     * @return Unique pointer to pathfinding
     */
    static std::unique_ptr<Pathfinding> Create();

    virtual ~Pathfinding() = default;

    /**
     * @brief Find path from start to goal
     * @param startX Start X position
     * @param startY Start Y position
     * @param startZ Start Z position
     * @param goalX Goal X position
     * @param goalY Goal Y position
     * @param goalZ Goal Z position
     * @return Path result
     */
    virtual Path FindPath(
        float startX, float startY, float startZ,
        float goalX, float goalY, float goalZ
    ) = 0;

    /**
     * @brief Build navigation mesh from terrain
     * @param terrainData Terrain heightmap
     * @param width Terrain width
     * @param height Terrain height
     * @param cellSize Size of each navigation cell
     */
    virtual void BuildNavMesh(
        const float* terrainData,
        int width,
        int height,
        float cellSize = 1.0f
    ) = 0;

    /**
     * @brief Mark area as non-walkable
     * @param centerX Center X position
     * @param centerZ Center Z position
     * @param radius Radius of non-walkable area
     */
    virtual void SetNonWalkable(float centerX, float centerZ, float radius) = 0;

    /**
     * @brief Check if position is walkable
     * @param x X position
     * @param y Y position
     * @param z Z position
     * @return true if walkable
     */
    virtual bool IsWalkable(float x, float y, float z) const = 0;

    /**
     * @brief Get terrain height at position
     * @param x X position
     * @param z Z position
     * @return Height (Y coordinate)
     */
    virtual float GetTerrainHeight(float x, float z) const = 0;
};

} // namespace NRE
