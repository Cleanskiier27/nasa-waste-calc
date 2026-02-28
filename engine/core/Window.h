#pragma once

#include <memory>
#include <string>

namespace NRE {

/**
 * @brief Window management for cross-platform windowing
 * 
 * Handles window creation, events, and input for different platforms
 */
class Window {
public:
    struct Properties {
        std::string title;
        unsigned int width;
        unsigned int height;
        bool fullscreen;
        bool vsync;
    };

    virtual ~Window() = default;

    /**
     * @brief Create a window
     * @param props Window properties
     * @return Unique pointer to window
     */
    static std::unique_ptr<Window> Create(const Properties& props = Properties{
        "Nature Reality Engine", 1920, 1080, false, true
    });

    /**
     * @brief Update window (poll events)
     */
    virtual void OnUpdate() = 0;

    /**
     * @brief Get window width
     * @return Width in pixels
     */
    virtual unsigned int GetWidth() const = 0;

    /**
     * @brief Get window height
     * @return Height in pixels
     */
    virtual unsigned int GetHeight() const = 0;

    /**
     * @brief Check if window should close
     * @return true if window should close
     */
    virtual bool ShouldClose() const = 0;

    /**
     * @brief Get native window handle (platform-specific)
     * @return Native window handle
     */
    virtual void* GetNativeWindow() const = 0;
};

} // namespace NRE
