#pragma once

#include <chrono>

namespace NRE {

/**
 * @brief Time management and delta time tracking
 * 
 * Handles frame timing, delta time, and time-based updates
 */
class Time {
public:
    /**
     * @brief Initialize time system
     */
    static void Initialize();

    /**
     * @brief Update time (call at start of each frame)
     */
    static void Update();

    /**
     * @brief Get time since last frame (delta time)
     * @return Delta time in seconds
     */
    static float GetDeltaTime();

    /**
     * @brief Get total time since application start
     * @return Time in seconds
     */
    static float GetTime();

    /**
     * @brief Get current frames per second
     * @return FPS
     */
    static float GetFPS();

    /**
     * @brief Get frame count since start
     * @return Total frame count
     */
    static unsigned long long GetFrameCount();

private:
    static std::chrono::high_resolution_clock::time_point s_StartTime;
    static std::chrono::high_resolution_clock::time_point s_LastFrameTime;
    static float s_DeltaTime;
    static float s_FPS;
    static unsigned long long s_FrameCount;
};

} // namespace NRE
