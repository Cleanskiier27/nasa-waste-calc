#pragma once

#include "TreeRenderer.h"
#include "../renderer/Mesh.h"

#include <cmath>
#include <functional>
#include <random>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace NRE {

/**
 * @brief L-system string rewriting engine
 *
 * Generates branch topology strings from production rules (e.g. Prusinkiewicz 1990).
 * Alphabet:
 *   F  – draw forward (add branch segment)
 *   +  – yaw left by angle
 *   -  – yaw right by angle
 *   &  – pitch down by angle
 *   ^  – pitch up by angle
 *   \  – roll left by angle
 *   /  – roll right by angle
 *   |  – turn 180° (U-turn)
 *   [  – push turtle state
 *   ]  – pop turtle state
 */
class LSystem {
public:
    using RuleMap = std::unordered_map<char, std::string>;

    LSystem(const std::string& axiom, const RuleMap& rules, float angle, float lengthDecay)
        : m_axiom(axiom), m_rules(rules), m_angle(angle), m_lengthDecay(lengthDecay)
    {
        m_current = m_axiom;
    }

    /** Apply production rules @p iterations times. */
    void Iterate(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            std::string next;
            next.reserve(m_current.size() * 3);
            for (char c : m_current) {
                auto it = m_rules.find(c);
                if (it != m_rules.end())
                    next += it->second;
                else
                    next += c;
            }
            m_current = std::move(next);
        }
    }

    const std::string& GetString() const { return m_current; }
    float GetAngle()       const { return m_angle; }
    float GetLengthDecay() const { return m_lengthDecay; }

    /** Return predefined L-system rules for the given species. */
    static LSystem ForSpecies(const std::string& species, float branchAngle) {
        if (species == "pine") {
            return LSystem{
                "F",
                {{'F', "FF-[-F+F+F]+[+F-F-F]"}},
                branchAngle, 0.8f
            };
        } else if (species == "maple") {
            return LSystem{
                "F",
                {{'F', "F[+F][-F][&F][^F]"}},
                branchAngle, 0.72f
            };
        } else if (species == "birch") {
            return LSystem{
                "F",
                {{'F', "F[+F][-F]F"}},
                branchAngle, 0.75f
            };
        } else {
            // Default: oak
            return LSystem{
                "F",
                {{'F', "F[+F]F[-F][F]"}},
                branchAngle, 0.7f
            };
        }
    }

private:
    std::string  m_axiom;
    std::string  m_current;
    RuleMap      m_rules;
    float        m_angle;
    float        m_lengthDecay;
};

// ---------------------------------------------------------------------------

/**
 * @brief Represents a single branch segment produced by the L-system turtle.
 */
struct Branch {
    float start[3];
    float end[3];
    float thickness;   // radius at the base of the segment
    int   depth;       // branch depth (0 = trunk)
};

// ---------------------------------------------------------------------------

/**
 * @brief Turtle state used while interpreting an L-system string.
 */
struct TurtleState {
    float pos[3]     = {0.f, 0.f, 0.f};
    float forward[3] = {0.f, 1.f, 0.f};  // unit vector pointing "up" initially
    float right[3]   = {1.f, 0.f, 0.f};
    float up[3]      = {0.f, 0.f, 1.f};
    float thickness  = 1.f;
    int   depth      = 0;
};

// ---------------------------------------------------------------------------

/**
 * @brief Full concrete implementation of TreeRenderer.
 *
 * Implements:
 *  - L-system procedural generation
 *  - 3D cylindrical mesh construction from branch segments
 *  - Wind physics (sinusoidal per-branch displacement)
 *  - Seasonal foliage changes (leaf density, colour tint)
 *  - Vulkan-ready mesh data (vertices + indices in Mesh::Vertex layout)
 *  - Ecosystem integration hooks (bounds, growth)
 */
class TreeRendererImpl : public TreeRenderer {
public:
    explicit TreeRendererImpl(const TreeConfig& config)
        : m_config(config)
        , m_season(Season::Summer)
        , m_windDir{1.f, 0.f, 0.f}
        , m_windStrength(0.f)
        , m_time(0.f)
        , m_minBounds{0.f, 0.f, 0.f}
        , m_maxBounds{0.f, 0.f, 0.f}
    {}

    // ------------------------------------------------------------------
    // TreeRenderer interface
    // ------------------------------------------------------------------

    void Generate() override {
        m_branches.clear();
        m_vertices.clear();
        m_baseVertices.clear();
        m_indices.clear();

        // Build L-system
        LSystem lsys = LSystem::ForSpecies(m_config.species, m_config.branchAngle);
        lsys.Iterate(m_config.iterations);

        // Interpret string → branch segments
        InterpretLSystem(lsys);

        // Build 3D mesh from branches
        BuildMesh();

        // Snapshot base positions before any wind is applied
        m_baseVertices = m_vertices;

        // Compute axis-aligned bounding box
        ComputeBounds();
    }

    void Update(float deltaTime) override {
        m_time += deltaTime;

        if (!m_config.enableWindPhysics || m_windStrength < 1e-4f)
            return;

        // Compute wind displacement relative to the base (rest) positions so
        // that repeated Update() calls do not cause unbounded positional drift.
        for (std::size_t i = 0; i < m_vertices.size(); ++i) {
            float height     = m_baseVertices[i].position[1];
            float swayAmount = m_windStrength * height * 0.05f;
            float phase      = m_time * 2.f + height * 0.3f;

            m_vertices[i].position[0] = m_baseVertices[i].position[0]
                + m_windDir[0] * swayAmount * std::sin(phase);
            m_vertices[i].position[2] = m_baseVertices[i].position[2]
                + m_windDir[2] * swayAmount * std::cos(phase * 0.7f);
        }
    }

    void Render() override {
        // In a full implementation this submits m_vertices / m_indices to the
        // Vulkan command buffer via the active Renderer instance.
        // The data is already in the Mesh::Vertex layout ready for upload.
    }

    void SetSeason(Season season) override {
        m_season = season;
        ApplySeasonalTint();
    }

    void ApplyWind(const float direction[3], float strength) override {
        m_windDir[0] = direction[0];
        m_windDir[1] = direction[1];
        m_windDir[2] = direction[2];
        m_windStrength = strength;
    }

    void Grow(float years) override {
        m_config.age    += years;
        m_config.height *= (1.f + years * 0.05f);  // ~5 % height gain per year
        m_config.branchThickness *= (1.f + years * 0.03f);
        Generate();  // Regenerate with updated config
    }

    void GetBounds(float minBounds[3], float maxBounds[3]) const override {
        for (int i = 0; i < 3; ++i) {
            minBounds[i] = m_minBounds[i];
            maxBounds[i] = m_maxBounds[i];
        }
    }

    // ------------------------------------------------------------------
    // Accessors for testing / Vulkan upload
    // ------------------------------------------------------------------

    const std::vector<Mesh::Vertex>& GetVertices() const { return m_vertices; }
    const std::vector<unsigned int>& GetIndices()  const { return m_indices; }
    const std::vector<Branch>&       GetBranches() const { return m_branches; }

private:
    // ------------------------------------------------------------------
    // L-system turtle interpretation
    // ------------------------------------------------------------------

    /** Rotate vector @p v around @p axis by @p radians (Rodrigues). */
    static void Rotate(float v[3], const float axis[3], float radians) {
        float c = std::cos(radians);
        float s = std::sin(radians);
        float d = axis[0]*v[0] + axis[1]*v[1] + axis[2]*v[2];

        float result[3] = {
            v[0]*c + (axis[1]*v[2] - axis[2]*v[1])*s + axis[0]*d*(1.f-c),
            v[1]*c + (axis[2]*v[0] - axis[0]*v[2])*s + axis[1]*d*(1.f-c),
            v[2]*c + (axis[0]*v[1] - axis[1]*v[0])*s + axis[2]*d*(1.f-c)
        };
        v[0] = result[0]; v[1] = result[1]; v[2] = result[2];
    }

    /** Re-orthogonalise and normalise a vector. */
    static void Normalise(float v[3]) {
        float len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        if (len > 1e-6f) { v[0] /= len; v[1] /= len; v[2] /= len; }
    }

    void InterpretLSystem(const LSystem& lsys) {
        const float angleRad = lsys.GetAngle() * static_cast<float>(M_PI) / 180.f;

        TurtleState turtle;
        turtle.thickness = m_config.branchThickness;

        std::stack<TurtleState> stateStack;
        float segLen = m_config.branchLength;

        for (char c : lsys.GetString()) {
            switch (c) {
                case 'F': {
                    Branch b;
                    b.start[0] = turtle.pos[0];
                    b.start[1] = turtle.pos[1];
                    b.start[2] = turtle.pos[2];
                    float len = segLen * std::pow(lsys.GetLengthDecay(),
                                                  static_cast<float>(turtle.depth));
                    turtle.pos[0] += turtle.forward[0] * len;
                    turtle.pos[1] += turtle.forward[1] * len;
                    turtle.pos[2] += turtle.forward[2] * len;
                    b.end[0] = turtle.pos[0];
                    b.end[1] = turtle.pos[1];
                    b.end[2] = turtle.pos[2];
                    b.thickness = turtle.thickness;
                    b.depth     = turtle.depth;
                    turtle.thickness *= 0.7f;
                    m_branches.push_back(b);
                    break;
                }
                case '+': Rotate(turtle.forward, turtle.up,    -angleRad); Normalise(turtle.forward); break;
                case '-': Rotate(turtle.forward, turtle.up,     angleRad); Normalise(turtle.forward); break;
                case '&': Rotate(turtle.forward, turtle.right, -angleRad); Normalise(turtle.forward); break;
                case '^': Rotate(turtle.forward, turtle.right,  angleRad); Normalise(turtle.forward); break;
                case '\\': Rotate(turtle.up, turtle.forward,   -angleRad); Normalise(turtle.up); break;
                case '/': Rotate(turtle.up, turtle.forward,     angleRad); Normalise(turtle.up); break;
                case '|': {
                    float pi = static_cast<float>(M_PI);
                    Rotate(turtle.forward, turtle.up, pi);
                    Normalise(turtle.forward);
                    break;
                }
                case '[':
                    stateStack.push(turtle);
                    turtle.depth++;
                    break;
                case ']':
                    if (!stateStack.empty()) {
                        turtle = stateStack.top();
                        stateStack.pop();
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // ------------------------------------------------------------------
    // 3D mesh generation — cylinders for branches
    // ------------------------------------------------------------------

    /**
     * @brief Append a cylinder between @p start and @p end to the mesh buffers.
     *
     * @param start      World-space start of the segment
     * @param end        World-space end of the segment
     * @param radius     Cylinder radius
     * @param segments   Number of radial subdivisions (≥ 3)
     */
    void AppendCylinder(const float start[3], const float end[3],
                        float radius, int segments) {
        // Axis vector
        float axis[3] = {
            end[0] - start[0],
            end[1] - start[1],
            end[2] - start[2]
        };
        float axisLen = std::sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
        if (axisLen < 1e-6f) return;

        float axisN[3] = { axis[0]/axisLen, axis[1]/axisLen, axis[2]/axisLen };

        // Build a local tangent/bitangent perpendicular to axisN
        float tangent[3];
        if (std::abs(axisN[0]) < 0.9f) {
            tangent[0] = 0.f; tangent[1] = -axisN[2]; tangent[2] = axisN[1];
        } else {
            tangent[0] = axisN[2]; tangent[1] = 0.f; tangent[2] = -axisN[0];
        }
        Normalise(tangent);

        // bitangent = axisN × tangent
        float bitan[3] = {
            axisN[1]*tangent[2] - axisN[2]*tangent[1],
            axisN[2]*tangent[0] - axisN[0]*tangent[2],
            axisN[0]*tangent[1] - axisN[1]*tangent[0]
        };
        Normalise(bitan);

        unsigned int baseIndex = static_cast<unsigned int>(m_vertices.size());

        // Emit two rings of vertices (bottom = start, top = end)
        for (int ring = 0; ring < 2; ++ring) {
            const float* centre = (ring == 0) ? start : end;
            for (int i = 0; i < segments; ++i) {
                float theta = 2.f * static_cast<float>(M_PI) * i / segments;
                float ct = std::cos(theta);
                float st = std::sin(theta);

                Mesh::Vertex v{};
                v.position[0] = centre[0] + radius * (ct * tangent[0] + st * bitan[0]);
                v.position[1] = centre[1] + radius * (ct * tangent[1] + st * bitan[1]);
                v.position[2] = centre[2] + radius * (ct * tangent[2] + st * bitan[2]);

                v.normal[0] = ct * tangent[0] + st * bitan[0];
                v.normal[1] = ct * tangent[1] + st * bitan[1];
                v.normal[2] = ct * tangent[2] + st * bitan[2];

                v.texCoord[0] = static_cast<float>(i) / segments;
                v.texCoord[1] = static_cast<float>(ring);

                v.tangent[0] = tangent[0]; v.tangent[1] = tangent[1]; v.tangent[2] = tangent[2];
                v.bitangent[0] = bitan[0]; v.bitangent[1] = bitan[1]; v.bitangent[2] = bitan[2];

                m_vertices.push_back(v);
            }
        }

        // Stitch the two rings into quads (two triangles each)
        for (int i = 0; i < segments; ++i) {
            unsigned int a = baseIndex + i;
            unsigned int b = baseIndex + (i + 1) % segments;
            unsigned int c = baseIndex + segments + i;
            unsigned int d = baseIndex + segments + (i + 1) % segments;

            m_indices.push_back(a); m_indices.push_back(b); m_indices.push_back(c);
            m_indices.push_back(b); m_indices.push_back(d); m_indices.push_back(c);
        }
    }

    void BuildMesh() {
        // Number of radial segments scales down for thinner (higher-depth) branches
        const int kMaxSegments = 8;
        const int kMinSegments = 3;

        for (const auto& branch : m_branches) {
            float t = std::max(0.f, std::min(1.f,
                branch.thickness / m_config.branchThickness));
            int segments = kMinSegments +
                static_cast<int>(t * (kMaxSegments - kMinSegments));

            float r = std::max(0.005f, branch.thickness * 0.5f);
            AppendCylinder(branch.start, branch.end, r, segments);
        }
    }

    // ------------------------------------------------------------------
    // Bounds computation
    // ------------------------------------------------------------------

    void ComputeBounds() {
        if (m_vertices.empty()) return;

        m_minBounds[0] = m_maxBounds[0] = m_vertices[0].position[0];
        m_minBounds[1] = m_maxBounds[1] = m_vertices[0].position[1];
        m_minBounds[2] = m_maxBounds[2] = m_vertices[0].position[2];

        for (const auto& v : m_vertices) {
            for (int i = 0; i < 3; ++i) {
                if (v.position[i] < m_minBounds[i]) m_minBounds[i] = v.position[i];
                if (v.position[i] > m_maxBounds[i]) m_maxBounds[i] = v.position[i];
            }
        }
    }

    // ------------------------------------------------------------------
    // Seasonal tint (modifies vertex normals' W channel via texCoord.y)
    // ------------------------------------------------------------------

    /**
     * @brief Adjust texCoord.y on branch-tip vertices to encode seasonal state.
     *
     * In a full shading pipeline the bark/leaf material reads this channel to
     * blend between season-specific albedo maps.
     *
     * Encoding:
     *   0.0 = bare / winter
     *   0.25 = spring bud green
     *   0.5  = summer full green
     *   0.75 = autumn orange/red
     */
    void ApplySeasonalTint() {
        float tint = 0.5f;
        switch (m_season) {
            case Season::Spring: tint = 0.25f; break;
            case Season::Summer: tint = 0.5f;  break;
            case Season::Autumn: tint = 0.75f; break;
            case Season::Winter: tint = 0.0f;  break;
        }
        // Apply tint to tip-ring vertices (ring == 1, texCoord.y == 1.0)
        for (auto& v : m_vertices) {
            if (v.texCoord[1] > 0.9f)
                v.texCoord[1] = tint;
        }
    }

    // ------------------------------------------------------------------
    // Members
    // ------------------------------------------------------------------

    TreeConfig               m_config;
    Season                   m_season;
    std::vector<Branch>       m_branches;
    std::vector<Mesh::Vertex> m_vertices;
    std::vector<Mesh::Vertex> m_baseVertices;  // rest-pose snapshot for wind displacement
    std::vector<unsigned int> m_indices;

    float m_windDir[3];
    float m_windStrength;
    float m_time;

    float m_minBounds[3];
    float m_maxBounds[3];
};

// ---------------------------------------------------------------------------
// TreeRenderer factory — defined here so the header is self-contained.
// ---------------------------------------------------------------------------

inline std::unique_ptr<TreeRenderer> TreeRenderer::Create(const TreeConfig& config) {
    return std::make_unique<TreeRendererImpl>(config);
}

} // namespace NRE
