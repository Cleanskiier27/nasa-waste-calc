#pragma once

#include <memory>
#include <string>

namespace NRE {

/**
 * @brief Core application class for Nature Reality Engine
 * 
 * Main entry point for engine initialization, update loop, and shutdown.
 * Manages window creation, input handling, and the main game loop.
 */
class Application {
public:
    /**
     * @brief Application configuration
     */
    struct Config {
        std::string title = "Nature Reality Engine";
        int width = 1920;
        int height = 1080;
        bool fullscreen = false;
        bool vsync = true;
        bool enableRayTracing = true;
        int targetFPS = 60;
    };

    /**
     * @brief Create application instance
     * @param config Application configuration
     * @return Unique pointer to application
     */
    static std::unique_ptr<Application> Create(const Config& config = Config{});

    virtual ~Application() = default;

    /**
     * @brief Initialize the application
     * @return true if initialization successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Run the main application loop
     */
    virtual void Run() = 0;

    /**
     * @brief Shutdown the application
     */
    virtual void Shutdown() = 0;

    /**
     * @brief Check if application should close
     * @return true if application should close
     */
    virtual bool ShouldClose() const = 0;

    /**
     * @brief Get delta time (time between frames)
     * @return Delta time in seconds
     */
    virtual float GetDeltaTime() const = 0;

    /**
     * @brief Get frames per second
     * @return Current FPS
     */
    virtual float GetFPS() const = 0;
};

} // namespace NRE
