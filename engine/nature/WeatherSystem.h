#pragma once

#include <memory>
#include <vector>
#include <string>

namespace NRE {

/**
 * @brief Weather simulation system with atmospheric effects
 * 
 * Real atmospheric simulation including clouds, rain, snow, wind, and lightning
 */
class WeatherSystem {
public:
    enum class WeatherType {
        Clear,
        PartlyCloudy,
        Cloudy,
        Overcast,
        LightRain,
        Rain,
        HeavyRain,
        Thunderstorm,
        LightSnow,
        Snow,
        Blizzard,
        Fog,
        Hail
    };

    struct Config {
        // Simulation
        bool enableAtmosphericSimulation = true;
        bool enableVolumetricClouds = true;
        bool enablePrecipitation = true;
        
        // Visual quality
        int cloudResolution = 512;
        int precipitationParticles = 100000;
        
        // Physical properties
        float temperature = 20.0f;      // Celsius
        float humidity = 0.5f;          // 0-1
        float windSpeed = 5.0f;         // m/s
        float windDirection[2] = {1.0f, 0.0f};
        float pressure = 1013.25f;      // Millibars
    };

    virtual ~WeatherSystem() = default;

    /**
     * @brief Create weather system
     * @param config Weather configuration
     * @return Unique pointer to weather system
     */
    static std::unique_ptr<WeatherSystem> Create(const Config& config = Config{});

    /**
     * @brief Initialize weather system
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Update weather simulation
     * @param deltaTime Time since last update
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Render weather effects
     */
    virtual void Render() = 0;

    /**
     * @brief Set weather type
     * @param type Weather type to set
     * @param transitionTime Time to transition (seconds)
     */
    virtual void SetWeather(WeatherType type, float transitionTime = 10.0f) = 0;

    /**
     * @brief Get current weather type
     * @return Current weather type
     */
    virtual WeatherType GetWeather() const = 0;

    /**
     * @brief Set time of day (affects sun position, lighting)
     * @param hour Hour (0-23)
     * @param minute Minute (0-59)
     */
    virtual void SetTimeOfDay(int hour, int minute) = 0;

    /**
     * @brief Set geographical location (affects sun angle)
     * @param latitude Latitude in degrees (-90 to 90)
     * @param longitude Longitude in degrees (-180 to 180)
     */
    virtual void SetLocation(float latitude, float longitude) = 0;

    /**
     * @brief Set date (affects season, day length)
     * @param year Year
     * @param month Month (1-12)
     * @param day Day (1-31)
     */
    virtual void SetDate(int year, int month, int day) = 0;

    /**
     * @brief Trigger lightning strike
     * @param x X position
     * @param y Y position
     * @param z Z position
     */
    virtual void TriggerLightning(float x, float y, float z) = 0;

    /**
     * @brief Get temperature at position
     * @param altitude Altitude in meters
     * @return Temperature in Celsius
     */
    virtual float GetTemperature(float altitude) const = 0;
};

} // namespace NRE
