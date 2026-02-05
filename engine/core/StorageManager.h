#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace NRE {

/**
 * @brief Storage management for persistent data on mobile and desktop platforms
 * 
 * Provides SSD/flash storage optimization with caching, compression, and
 * platform-specific optimizations for iOS, Android, and other platforms.
 */
class StorageManager {
public:
    enum class Platform {
        Desktop,
        iOS,
        Android,
        Web
    };

    enum class CompressionType {
        None,
        LZ4,        // Fast compression
        ZSTD,       // High compression ratio
        Auto        // Choose based on data type
    };

    struct Config {
        Platform platform = Platform::Desktop;
        std::string storagePath = "./storage";
        size_t maxCacheSizeMB = 512;            // Maximum cache size in MB
        bool enableCompression = true;
        CompressionType compression = CompressionType::Auto;
        bool enableEncryption = false;           // Encrypt sensitive data
        size_t writeBufferSizeKB = 64;          // Write buffer for SSD optimization
        bool enableWearLeveling = true;          // SSD wear leveling
    };

    /**
     * @brief Storage priority for different types of data
     */
    enum class Priority {
        Critical,       // Always keep in memory and storage (save games, settings)
        High,          // Keep in storage, load on demand (game assets)
        Medium,        // Can be evicted from cache (texture mips)
        Low            // Can be regenerated (procedural content)
    };

    /**
     * @brief Create storage manager instance
     * @param config Storage configuration
     * @return Unique pointer to storage manager
     */
    static std::unique_ptr<StorageManager> Create(const Config& config);

    virtual ~StorageManager() = default;

    /**
     * @brief Initialize storage system
     * @return true if successful
     */
    virtual bool Initialize() = 0;

    /**
     * @brief Write data to persistent storage
     * @param key Unique identifier for the data
     * @param data Data buffer to write
     * @param size Size of data in bytes
     * @param priority Storage priority
     * @return true if successful
     */
    virtual bool Write(const std::string& key, const void* data, size_t size, 
                       Priority priority = Priority::High) = 0;

    /**
     * @brief Read data from storage
     * @param key Unique identifier for the data
     * @param data Buffer to read data into
     * @param size Size of buffer
     * @return Number of bytes read, 0 if not found
     */
    virtual size_t Read(const std::string& key, void* data, size_t size) = 0;

    /**
     * @brief Check if data exists in storage
     * @param key Unique identifier for the data
     * @return true if exists
     */
    virtual bool Exists(const std::string& key) = 0;

    /**
     * @brief Delete data from storage
     * @param key Unique identifier for the data
     * @return true if successful
     */
    virtual bool Delete(const std::string& key) = 0;

    /**
     * @brief Get size of stored data
     * @param key Unique identifier for the data
     * @return Size in bytes, 0 if not found
     */
    virtual size_t GetSize(const std::string& key) = 0;

    /**
     * @brief Clear all cached data (keeps persistent data)
     */
    virtual void ClearCache() = 0;

    /**
     * @brief Get current cache usage
     * @return Cache usage in bytes
     */
    virtual size_t GetCacheUsage() const = 0;

    /**
     * @brief Get total storage usage
     * @return Storage usage in bytes
     */
    virtual size_t GetStorageUsage() const = 0;

    /**
     * @brief Optimize storage (defragment, garbage collect)
     * Mobile-friendly: Respects battery and thermal constraints
     */
    virtual void Optimize() = 0;

    /**
     * @brief Flush all pending writes to storage
     * Important for SSD wear leveling and data integrity
     */
    virtual void Flush() = 0;

    /**
     * @brief List all keys in storage
     * @return Vector of all stored keys
     */
    virtual std::vector<std::string> ListKeys() const = 0;

    /**
     * @brief Set callback for low storage warning
     * @param callback Function to call when storage is low
     */
    virtual void SetLowStorageCallback(std::function<void(size_t bytesRemaining)> callback) = 0;

    /**
     * @brief Shutdown storage system
     */
    virtual void Shutdown() = 0;
};

} // namespace NRE
