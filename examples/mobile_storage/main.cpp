#include <core/StorageManager.h>
#include "../runtime/GameLoader.h"
#include <iostream>
#include <vector>

using namespace NRE;

// Mock implementation for demonstration
class MockStorageManager : public StorageManager {
public:
    bool Initialize() override { return true; }
    bool Write(const std::string& key, const void* data, size_t size, Priority priority) override { return true; }
    size_t Read(const std::string& key, void* data, size_t size) override { return 0; }
    bool Exists(const std::string& key) override { return false; }
    bool Delete(const std::string& key) override { return true; }
    size_t GetSize(const std::string& key) override { return 0; }
    void ClearCache() override {}
    size_t GetCacheUsage() const override { return 0; }
    size_t GetStorageUsage() const override { return 0; }
    void Optimize() override {}
    void Flush() override {}
    std::vector<std::string> ListKeys() const override { return {}; }
    void SetLowStorageCallback(std::function<void(size_t)> callback) override {}
    void Shutdown() override {}
};

std::unique_ptr<StorageManager> StorageManager::Create(const Config& config) {
    return std::make_unique<MockStorageManager>();
}

// Mock GameLoader for demonstration
class MockGameLoader : public GameLoader {
public:
    class MockGame : public Game {
    public:
        void ApplyEnhancements(const EnhancementOptions& options) override {}
        void Run() override {}
        void Pause() override {}
        void Resume() override {}
        void Stop() override {}
        std::string GetTitle() const override { return "Mock Game"; }
        Platform GetPlatform() const override { return Platform::Android; }
    };
    
    std::unique_ptr<Game> LoadGame(const std::string& path, Platform platform) override {
        return std::make_unique<MockGame>();
    }
};

std::unique_ptr<GameLoader> GameLoader::Create() {
    return std::make_unique<MockGameLoader>();
}

/**
 * @brief Example: Using SSD storage optimization for mobile/phone platforms
 * 
 * This example demonstrates how to configure and use the StorageManager
 * for optimal performance on mobile devices (iOS, Android) with SSD/flash storage.
 */
int main() {
    std::cout << "=== Nature Reality Engine - Mobile SSD Storage Demo ===" << std::endl;

    // Configure storage for mobile phone (iOS/Android)
    StorageManager::Config storageConfig;
    storageConfig.platform = StorageManager::Platform::Android;  // or iOS
    storageConfig.maxCacheSizeMB = 256;  // Limit cache for mobile
    storageConfig.enableCompression = true;
    storageConfig.compression = StorageManager::CompressionType::LZ4;  // Fast compression for mobile
    storageConfig.enableWearLeveling = true;  // Protect SSD/flash memory
    storageConfig.writeBufferSizeKB = 32;  // Smaller buffer for mobile
    storageConfig.storagePath = "/sdcard/NatureRealityEngine/storage";  // Android path

    // Create storage manager
    auto storage = StorageManager::Create(storageConfig);
    if (!storage->Initialize()) {
        std::cerr << "Failed to initialize storage manager!" << std::endl;
        return 1;
    }

    std::cout << "Storage manager initialized for mobile device" << std::endl;

    // Example: Save game state to SSD
    struct SaveGame {
        int level = 5;
        int score = 12345;
        float playerX = 100.5f;
        float playerY = 200.3f;
    };

    SaveGame saveData;
    storage->Write("save_slot_1", &saveData, sizeof(SaveGame), 
                   StorageManager::Priority::Critical);
    
    std::cout << "Game state written to SSD storage" << std::endl;

    // Example: Store texture data with compression
    std::vector<uint8_t> textureData(1024 * 1024 * 4);  // 1MB texture
    storage->Write("texture_grass_01", textureData.data(), textureData.size(),
                   StorageManager::Priority::High);

    std::cout << "Texture data written with compression" << std::endl;

    // Example: Load game with SSD-optimized storage
    auto runtime = GameLoader::Create();
    auto game = runtime->LoadGame("mobile_game.apk", GameLoader::Platform::Android);
    
    GameLoader::EnhancementOptions options;
    options.textures = GameLoader::TextureQuality::Enhanced2K;  // Lower for mobile
    options.lighting = GameLoader::LightingMode::Enhanced;      // Lighter for mobile
    options.enableSSDOptimization = true;  // Enable storage optimization
    options.cacheSizeMB = 256;             // Mobile-friendly cache size
    options.compressAssets = true;         // Compress to save space
    
    game->ApplyEnhancements(options);
    
    std::cout << "Game loaded with mobile SSD optimizations" << std::endl;

    // Check storage usage
    size_t cacheUsage = storage->GetCacheUsage();
    size_t totalUsage = storage->GetStorageUsage();
    std::cout << "Cache usage: " << (cacheUsage / 1024 / 1024) << " MB" << std::endl;
    std::cout << "Total storage: " << (totalUsage / 1024 / 1024) << " MB" << std::endl;

    // Set callback for low storage warning
    storage->SetLowStorageCallback([](size_t bytesRemaining) {
        std::cout << "WARNING: Low storage space! " 
                  << (bytesRemaining / 1024 / 1024) << " MB remaining" << std::endl;
    });

    // Optimize storage (defragment, garbage collect)
    std::cout << "Optimizing SSD storage..." << std::endl;
    storage->Optimize();

    // List all stored data
    auto keys = storage->ListKeys();
    std::cout << "Stored items:" << std::endl;
    for (const auto& key : keys) {
        size_t size = storage->GetSize(key);
        std::cout << "  - " << key << " (" << size << " bytes)" << std::endl;
    }

    // Flush and shutdown
    storage->Flush();
    storage->Shutdown();
    
    std::cout << "Storage flushed and shutdown complete" << std::endl;
    std::cout << "=== Demo Complete ===" << std::endl;

    return 0;
}
