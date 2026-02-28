#pragma once

#include <functional>

namespace NRE {

/**
 * @brief Input system for keyboard, mouse, and controller input
 * 
 * Cross-platform input handling with callback support
 */
class Input {
public:
    enum class Key {
        W, A, S, D,
        Space, Escape, Enter,
        Up, Down, Left, Right,
        Mouse_Left, Mouse_Right, Mouse_Middle,
        // Add more as needed
    };

    enum class KeyState {
        Pressed,
        Released,
        Repeat
    };

    /**
     * @brief Initialize input system
     * @return true if successful
     */
    static bool Initialize();

    /**
     * @brief Shutdown input system
     */
    static void Shutdown();

    /**
     * @brief Update input state (call each frame)
     */
    static void Update();

    /**
     * @brief Check if key is currently pressed
     * @param key Key to check
     * @return true if pressed
     */
    static bool IsKeyPressed(Key key);

    /**
     * @brief Check if key was just pressed this frame
     * @param key Key to check
     * @return true if just pressed
     */
    static bool IsKeyJustPressed(Key key);

    /**
     * @brief Get mouse position
     * @param x Output X coordinate
     * @param y Output Y coordinate
     */
    static void GetMousePosition(float& x, float& y);

    /**
     * @brief Get mouse delta (movement since last frame)
     * @param dx Output delta X
     * @param dy Output delta Y
     */
    static void GetMouseDelta(float& dx, float& dy);

    /**
     * @brief Set key callback function
     * @param callback Function to call on key event
     */
    using KeyCallback = std::function<void(Key, KeyState)>;
    static void SetKeyCallback(KeyCallback callback);
};

} // namespace NRE
