#pragma once

#include <memory>
#include <string>
#include <functional>

namespace NRE {

/**
 * @brief Universal game loader - Run ANY game from ANY platform
 * 
 * Like Proton (Steam's Windows compatibility) but for all platforms.
 * Loads games, translates APIs, and applies photorealistic enhancements.
 */
class GameLoader {
public:
    enum class Platform {
        // PC
        Windows_DirectX11,
        Windows_DirectX12,
        Windows_OpenGL,
        Windows_Vulkan,
        MacOS_Metal,
        MacOS_OpenGL,
        Linux_Vulkan,
        Linux_OpenGL,
        
        // Consoles
        PlayStation5,
        XboxSeriesX,
        NintendoSwitch,
        PlayStation4,
        XboxOne,
        
        // Mobile
        iOS,
        Android,
        
        // Retro
        NES,
        SNES,
        Nintendo64,
        PlayStation1,
        PlayStation2,
        GameCube,
        Wii,
        
        // VR
        OculusQuest,
        PSVR2,
        ValveIndex,
        AppleVisionPro,
        
        // Web
        HTML5,
        WebGL,
        WebGPU,
        Flash,
        
        // Auto-detect
        AutoDetect
    };

    enum class TextureQuality {
        Original,               // Keep original textures
        Enhanced2K,             // Upscale to 2K
        Enhanced4K,             // Upscale to 4K
        Photorealistic8K        // Full photorealistic 8K
    };

    enum class LightingMode {
        Original,               // Keep original lighting
        Enhanced,               // Better shadows, more lights
        RayTraced,              // Full ray tracing
        PathTraced              // Path tracing (cinema quality)
    };

    enum class WaterMode {
        Original,               // Keep original water
        Enhanced,               // Better reflections
        FluidSimulation         // Full Navier-Stokes simulation
    };

    enum class VegetationMode {
        Original,               // Keep original vegetation
        Enhanced,               // More grass/trees
        ProceduralMillions      // Millions of procedural grass/trees
    };

    enum class WildlifeMode {
        None,                   // No wildlife
        Basic,                  // Simple ambient animals
        RealisticAI             // Full ecosystem with behavior AI
    };

    struct EnhancementOptions {
        TextureQuality textures = TextureQuality::Enhanced4K;
        LightingMode lighting = LightingMode::RayTraced;
        WaterMode water = WaterMode::FluidSimulation;
        VegetationMode vegetation = VegetationMode::ProceduralMillions;
        WildlifeMode wildlife = WildlifeMode::RealisticAI;
        bool dynamicWeather = true;
        bool realisticPhysics = true;
        bool spatialAudio = true;
        
        // Mobile/Phone storage optimization
        bool enableSSDOptimization = true;   // Enable SSD/flash storage optimizations
        size_t cacheSizeMB = 256;            // Cache size for mobile devices
        bool compressAssets = true;          // Compress stored assets to save space
    };

    /**
     * @brief Game interface for loaded games
     */
    class Game {
    public:
        virtual ~Game() = default;

        /**
         * @brief Apply photorealistic enhancements
         * @param options Enhancement options
         */
        virtual void ApplyEnhancements(const EnhancementOptions& options) = 0;

        /**
         * @brief Run the game
         */
        virtual void Run() = 0;

        /**
         * @brief Pause the game
         */
        virtual void Pause() = 0;

        /**
         * @brief Resume the game
         */
        virtual void Resume() = 0;

        /**
         * @brief Stop the game
         */
        virtual void Stop() = 0;

        /**
         * @brief Get game title
         * @return Game title
         */
        virtual std::string GetTitle() const = 0;

        /**
         * @brief Get detected platform
         * @return Platform
         */
        virtual Platform GetPlatform() const = 0;
    };

    /**
     * @brief Create game loader instance
     * @return Unique pointer to game loader
     */
    static std::unique_ptr<GameLoader> Create();

    virtual ~GameLoader() = default;

    /**
     * @brief Load game from file
     * @param path Path to game executable/ROM/APK
     * @param platform Platform (AutoDetect to detect automatically)
     * @return Unique pointer to loaded game
     */
    virtual std::unique_ptr<Game> LoadGame(
        const std::string& path,
        Platform platform = Platform::AutoDetect
    ) = 0;

    /**
     * @brief Check if platform is supported
     * @param platform Platform to check
     * @return true if supported
     */
    static bool IsPlatformSupported(Platform platform);

    /**
     * @brief Detect platform from file
     * @param path Path to game file
     * @return Detected platform
     */
    static Platform DetectPlatform(const std::string& path);
};

} // namespace NRE
