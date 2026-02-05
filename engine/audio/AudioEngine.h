#pragma once

#include <memory>
#include <string>
#include <functional>

namespace NRE {

/**
 * @brief 3D spatial audio engine with HRTF
 * 
 * Provides realistic 3D audio with Head-Related Transfer Function,
 * environmental acoustics, and Doppler effects
 */
class AudioEngine {
public:
    struct Sound {
        std::string name;
        float duration;         // Seconds
        int sampleRate;         // Hz (e.g., 44100)
        int channels;           // 1 = mono, 2 = stereo
        bool isLooping;
        float baseVolume;       // 0-1
    };

    struct SoundInstance {
        int id;                 // Unique instance ID
        float position[3];      // World position
        float velocity[3];      // For Doppler effect
        float volume;           // Current volume (0-1)
        float pitch;            // Pitch multiplier (1.0 = normal)
        bool isPlaying;
    };

    struct ReverbSettings {
        float reverbTime;       // Seconds (decay time)
        float roomSize;         // Meters
        float damping;          // High frequency damping (0-1)
        float wetMix;           // Reverb amount (0-1)
    };

    /**
     * @brief Create audio engine
     * @return Unique pointer to audio engine
     */
    static std::unique_ptr<AudioEngine> Create();

    virtual ~AudioEngine() = default;

    /**
     * @brief Initialize audio engine
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Update audio (call each frame)
     * @param listenerX Listener X position (camera)
     * @param listenerY Listener Y position
     * @param listenerZ Listener Z position
     * @param forwardX Forward direction X
     * @param forwardY Forward direction Y
     * @param forwardZ Forward direction Z
     * @param upX Up direction X
     * @param upY Up direction Y
     * @param upZ Up direction Z
     */
    virtual void Update(
        float listenerX, float listenerY, float listenerZ,
        float forwardX, float forwardY, float forwardZ,
        float upX, float upY, float upZ
    ) = 0;

    /**
     * @brief Load sound from file
     * @param path Path to audio file (WAV, MP3, OGG, etc.)
     * @param name Name to identify sound
     * @return true if successful
     */
    virtual bool LoadSound(const std::string& path, const std::string& name) = 0;

    /**
     * @brief Play sound at 3D position
     * @param name Sound name
     * @param x X position
     * @param y Y position
     * @param z Z position
     * @param volume Volume (0-1)
     * @param loop Should loop
     * @return Sound instance ID (-1 if failed)
     */
    virtual int PlaySound3D(
        const std::string& name,
        float x, float y, float z,
        float volume = 1.0f,
        bool loop = false
    ) = 0;

    /**
     * @brief Play sound (non-spatial, 2D)
     * @param name Sound name
     * @param volume Volume (0-1)
     * @param loop Should loop
     * @return Sound instance ID (-1 if failed)
     */
    virtual int PlaySound2D(
        const std::string& name,
        float volume = 1.0f,
        bool loop = false
    ) = 0;

    /**
     * @brief Stop sound instance
     * @param instanceId Instance ID from PlaySound
     */
    virtual void StopSound(int instanceId) = 0;

    /**
     * @brief Update sound position (for moving sources)
     * @param instanceId Instance ID
     * @param x New X position
     * @param y New Y position
     * @param z New Z position
     */
    virtual void UpdateSoundPosition(int instanceId, float x, float y, float z) = 0;

    /**
     * @brief Update sound velocity (for Doppler effect)
     * @param instanceId Instance ID
     * @param vx Velocity X
     * @param vy Velocity Y
     * @param vz Velocity Z
     */
    virtual void UpdateSoundVelocity(int instanceId, float vx, float vy, float vz) = 0;

    /**
     * @brief Set reverb settings for environment
     * @param settings Reverb settings
     */
    virtual void SetReverb(const ReverbSettings& settings) = 0;

    /**
     * @brief Set master volume
     * @param volume Master volume (0-1)
     */
    virtual void SetMasterVolume(float volume) = 0;

    /**
     * @brief Shutdown audio engine
     */
    virtual void Shutdown() = 0;
};

} // namespace NRE
