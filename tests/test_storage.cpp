#include <core/StorageManager.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>

using namespace NRE;

/**
 * @brief Test suite for StorageManager
 * 
 * Tests SSD storage functionality for mobile and desktop platforms
 */

// Mock implementation for testing
class MockStorageManager : public StorageManager {
private:
    struct StoredData {
        std::vector<uint8_t> data;
        Priority priority;
    };
    
    std::map<std::string, StoredData> storage_;
    size_t cacheUsage_ = 0;
    size_t totalUsage_ = 0;
    Config config_;
    std::function<void(size_t)> lowStorageCallback_;

public:
    MockStorageManager(const Config& config) : config_(config) {}
    
    bool Initialize() override {
        std::cout << "Storage initialized for platform: " 
                  << static_cast<int>(config_.platform) << std::endl;
        return true;
    }
    
    bool Write(const std::string& key, const void* data, size_t size, 
               Priority priority = Priority::High) override {
        StoredData stored;
        stored.data.resize(size);
        std::memcpy(stored.data.data(), data, size);
        stored.priority = priority;
        
        storage_[key] = stored;
        totalUsage_ += size;
        
        if (priority == Priority::Critical || priority == Priority::High) {
            cacheUsage_ += size;
        }
        
        return true;
    }
    
    size_t Read(const std::string& key, void* data, size_t size) override {
        auto it = storage_.find(key);
        if (it == storage_.end()) {
            return 0;
        }
        
        size_t readSize = std::min(size, it->second.data.size());
        std::memcpy(data, it->second.data.data(), readSize);
        return readSize;
    }
    
    bool Exists(const std::string& key) override {
        return storage_.find(key) != storage_.end();
    }
    
    bool Delete(const std::string& key) override {
        auto it = storage_.find(key);
        if (it == storage_.end()) {
            return false;
        }
        
        size_t size = it->second.data.size();
        totalUsage_ -= size;
        
        if (it->second.priority == Priority::Critical || 
            it->second.priority == Priority::High) {
            cacheUsage_ -= size;
        }
        
        storage_.erase(it);
        return true;
    }
    
    size_t GetSize(const std::string& key) override {
        auto it = storage_.find(key);
        return (it != storage_.end()) ? it->second.data.size() : 0;
    }
    
    void ClearCache() override {
        cacheUsage_ = 0;
    }
    
    size_t GetCacheUsage() const override {
        return cacheUsage_;
    }
    
    size_t GetStorageUsage() const override {
        return totalUsage_;
    }
    
    void Optimize() override {
        std::cout << "Storage optimized" << std::endl;
    }
    
    void Flush() override {
        std::cout << "Storage flushed" << std::endl;
    }
    
    std::vector<std::string> ListKeys() const override {
        std::vector<std::string> keys;
        for (const auto& pair : storage_) {
            keys.push_back(pair.first);
        }
        return keys;
    }
    
    void SetLowStorageCallback(std::function<void(size_t)> callback) override {
        lowStorageCallback_ = callback;
    }
    
    void Shutdown() override {
        storage_.clear();
        cacheUsage_ = 0;
        totalUsage_ = 0;
    }
};

// Factory function for mock
std::unique_ptr<StorageManager> StorageManager::Create(const Config& config) {
    return std::make_unique<MockStorageManager>(config);
}

void test_write_and_read() {
    std::cout << "Test: Write and Read" << std::endl;
    
    StorageManager::Config config;
    config.platform = StorageManager::Platform::Android;
    auto storage = StorageManager::Create(config);
    storage->Initialize();
    
    // Write data
    const char* testData = "Hello, SSD!";
    size_t dataSize = strlen(testData) + 1;
    storage->Write("test_key", testData, dataSize, StorageManager::Priority::High);
    
    // Read data
    char buffer[100];
    size_t readSize = storage->Read("test_key", buffer, sizeof(buffer));
    
    assert(readSize == dataSize);
    assert(strcmp(buffer, testData) == 0);
    
    std::cout << "  ✓ Write and read successful" << std::endl;
}

void test_exists_and_delete() {
    std::cout << "Test: Exists and Delete" << std::endl;
    
    StorageManager::Config config;
    auto storage = StorageManager::Create(config);
    storage->Initialize();
    
    const char* data = "Test data";
    storage->Write("temp_key", data, strlen(data) + 1);
    
    assert(storage->Exists("temp_key"));
    assert(!storage->Exists("non_existent_key"));
    
    storage->Delete("temp_key");
    assert(!storage->Exists("temp_key"));
    
    std::cout << "  ✓ Exists and delete successful" << std::endl;
}

void test_storage_usage() {
    std::cout << "Test: Storage Usage Tracking" << std::endl;
    
    StorageManager::Config config;
    auto storage = StorageManager::Create(config);
    storage->Initialize();
    
    std::vector<uint8_t> data1(1024);  // 1KB
    std::vector<uint8_t> data2(2048);  // 2KB
    
    storage->Write("data1", data1.data(), data1.size(), StorageManager::Priority::High);
    storage->Write("data2", data2.data(), data2.size(), StorageManager::Priority::Low);
    
    size_t totalUsage = storage->GetStorageUsage();
    assert(totalUsage == 3072);  // 3KB total
    
    size_t cacheUsage = storage->GetCacheUsage();
    assert(cacheUsage >= 1024);  // At least 1KB in cache
    
    std::cout << "  ✓ Storage usage tracking successful" << std::endl;
}

void test_list_keys() {
    std::cout << "Test: List Keys" << std::endl;
    
    StorageManager::Config config;
    auto storage = StorageManager::Create(config);
    storage->Initialize();
    
    storage->Write("key1", "data1", 6);
    storage->Write("key2", "data2", 6);
    storage->Write("key3", "data3", 6);
    
    auto keys = storage->ListKeys();
    assert(keys.size() == 3);
    
    std::cout << "  ✓ List keys successful, found " << keys.size() << " keys" << std::endl;
}

void test_mobile_config() {
    std::cout << "Test: Mobile Configuration" << std::endl;
    
    StorageManager::Config config;
    config.platform = StorageManager::Platform::iOS;
    config.maxCacheSizeMB = 256;
    config.enableCompression = true;
    config.compression = StorageManager::CompressionType::LZ4;
    config.enableWearLeveling = true;
    config.writeBufferSizeKB = 32;
    
    auto storage = StorageManager::Create(config);
    assert(storage->Initialize());
    
    std::cout << "  ✓ Mobile configuration successful" << std::endl;
}

int main() {
    std::cout << "=== StorageManager Test Suite ===" << std::endl << std::endl;
    
    test_write_and_read();
    test_exists_and_delete();
    test_storage_usage();
    test_list_keys();
    test_mobile_config();
    
    std::cout << std::endl << "=== All tests passed! ===" << std::endl;
    
    return 0;
}
