#pragma once

#include <memory>
#include <vector>
<string>
#include <functional>

namespace NRE {

/**
 * @brief Ecosystem simulation with living flora and fauna
 * 
 * Implements realistic plant growth, animal behavior, and predator-prey dynamics
 */
class EcosystemSimulation {
public:
    /**
     * @brief Plant species with growth simulation
     */
    class Plant {
    public:
        struct State {
            float sunlightExposure = 8.0f;     // Hours per day
            float waterAvailable = 100.0f;     // Liters in soil
            float soilNitrogen = 50.0f;        // N level (0-100)
            float soilPhosphorus = 50.0f;      // P level (0-100)
            float soilPotassium = 50.0f;       // K level (0-100)
            float temperature = 20.0f;         // Celsius
            int age = 0;                       // Days old
            float height = 0.1f;               // Meters
            int leafCount = 10;
            float health = 1.0f;               // 0-1
        };

        /**
         * @brief Photosynthesis: Convert light + CO2 → glucose + O2
         * @param sunlight Sunlight intensity (0-1)
         * @param co2 CO2 availability (0-1)
         * @return Glucose produced
         */
        virtual float Photosynthesize(float sunlight, float co2) = 0;

        /**
         * @brief Grow plant based on available resources
         * @param deltaTime Time since last update (days)
         */
        virtual void Grow(float deltaTime) = 0;

        /**
         * @brief Reproduce (create seeds when mature)
         * @return true if reproduction successful
         */
        virtual bool Reproduce() = 0;

        /**
         * @brief Die and decompose, returning nutrients to soil
         */
        virtual void Die() = 0;

        /**
         * @brief Get current state
         * @return Plant state
         */
        virtual const State& GetState() const = 0;
    };

    /**
     * @brief Animal species with behavior AI
     */
    class Animal {
    public:
        struct Needs {
            float hunger = 0.0f;      // 0-100 (100 = starving)
            float thirst = 0.0f;      // 0-100 (100 = dehydrated)
            float energy = 100.0f;    // 0-100 (0 = exhausted)
            float fear = 0.0f;        // 0-100 (100 = terrified)
            float reproductionDrive = 0.0f;  // 0-100
        };

        enum class Action {
            Idle,
            Eat,
            Drink,
            Sleep,
            Flee,
            Hunt,
            Mate,
            Wander,
            Patrol
        };

        /**
         * @brief Decide next action using utility-based AI
         * @return Action to perform
         */
        virtual Action DecideAction() = 0;

        /**
         * @brief Navigate to target position (A* pathfinding)
         * @param targetX Target X position
         * @param targetY Target Y position
         * @param targetZ Target Z position
         * @return true if path found
         */
        virtual bool NavigateToTarget(float targetX, float targetY, float targetZ) = 0;

        /**
         * @brief Detect threats in environment
         * @return List of threat positions
         */
        virtual std::vector<float*> DetectThreats() = 0;

        /**
         * @brief Update animal state
         * @param deltaTime Time since last update (seconds)
         */
        virtual void Update(float deltaTime) = 0;

        /**
         * @brief Get current needs
         * @return Animal needs
         */
        virtual const Needs& GetNeeds() const = 0;

        /**
         * @brief Get position
         * @param x Output X position
         * @param y Output Y position
         * @param z Output Z position
         */
        virtual void GetPosition(float& x, float& y, float& z) const = 0;
    };

    struct Config {
        // Simulation parameters
        float timeScale = 1.0f;         // Speed multiplier
        bool enablePlantGrowth = true;
        bool enableAnimalBehavior = true;
        bool enablePredatorPreyDynamics = true;
        
        // Population limits
        int maxPlants = 100000;
        int maxAnimals = 10000;
        
        // Environmental factors
        float baseTemperature = 20.0f;  // Celsius
        float rainfall = 1000.0f;       // mm per year
        float sunlightHours = 12.0f;    // Average hours per day
    };

    virtual ~EcosystemSimulation() = default;

    /**
     * @brief Create ecosystem simulation
     * @param config Ecosystem configuration
     * @return Unique pointer to ecosystem
     */
    static std::unique_ptr<EcosystemSimulation> Create(const Config& config = Config{});

    /**
     * @brief Initialize ecosystem
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Update ecosystem simulation
     * @param deltaTime Time since last update (seconds)
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Add plant to ecosystem
     * @param species Species name (e.g., "oak", "grass", "rose")
     * @param x X position
     * @param y Y position
     * @param z Z position
     * @return Pointer to created plant
     */
    virtual Plant* AddPlant(const std::string& species, float x, float y, float z) = 0;

    /**
     * @brief Add animal to ecosystem
     * @param species Species name (e.g., "deer", "wolf", "rabbit")
     * @param x X position
     * @param y Y position
     * @param z Z position
     * @return Pointer to created animal
     */
    virtual Animal* AddAnimal(const std::string& species, float x, float y, float z) = 0;

    /**
     * @brief Get all animals in ecosystem
     * @return Vector of animal pointers
     */
    virtual std::vector<Animal*> GetAnimals() = 0;

    /**
     * @brief Get all plants in ecosystem
     * @return Vector of plant pointers
     */
    virtual std::vector<Plant*> GetPlants() = 0;

    /**
     * @brief Get population count by species
     * @param species Species name
     * @return Population count
     */
    virtual int GetPopulation(const std::string& species) const = 0;
};

} // namespace NRE
