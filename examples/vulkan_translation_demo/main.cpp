/**
 * @file main.cpp
 * @brief Nature Reality Engine - Vulkan Renderer + DirectX → Vulkan API Translation Demo
 *
 * This example demonstrates:
 *  1. Vulkan instance/device initialization with validation layers
 *  2. WSL-compatible surface extension selection (xcb / wayland / headless)
 *  3. A DX11 → Vulkan API translation mapping table (educational)
 *  4. Simulated multi-frame rendering loop
 *
 * Platform: Linux / WSL1 / WSL2 (bare-metal Linux also supported)
 * C++ Standard: C++20
 * Build: see CMakeLists.txt in this directory
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <functional>
#include <optional>

// ── Vulkan ────────────────────────────────────────────────────────────────────
// VULKAN_SDK_AVAILABLE is defined by the demo's CMakeLists only when the
// Vulkan SDK (headers + loader) was found at configure time.  This is
// separate from the global USE_VULKAN flag so we don't try to include
// vulkan.h when the SDK is absent.
#ifdef VULKAN_SDK_AVAILABLE
#  include <vulkan/vulkan.h>
#endif

// ── ANSI colour helpers ───────────────────────────────────────────────────────
namespace ANSI {
    constexpr const char* RESET  = "\033[0m";
    constexpr const char* BOLD   = "\033[1m";
    constexpr const char* GREEN  = "\033[32m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* CYAN   = "\033[36m";
    constexpr const char* RED    = "\033[31m";
}

// ─────────────────────────────────────────────────────────────────────────────
// DX11 → Vulkan translation layer (educational / demonstration)
// ─────────────────────────────────────────────────────────────────────────────
namespace NRE {

class DX11ToVulkanTranslator {
public:
    struct TranslationEntry {
        std::string dx11Function;
        std::string vulkanFunction;
        std::string description;
    };

    DX11ToVulkanTranslator() {
        // Populate the mapping table
        mappings_ = {
            { "ID3D11Device::CreateTexture2D",
              "vkCreateImage",
              "Allocate a 2-D texture / render target" },

            { "ID3D11DeviceContext::Draw",
              "vkCmdDraw",
              "Non-indexed draw call" },

            { "ID3D11DeviceContext::DrawIndexed",
              "vkCmdDrawIndexed",
              "Indexed draw call" },

            { "ID3D11Device::CreateBuffer",
              "vkCreateBuffer + vkAllocateMemory",
              "Create a vertex, index, or constant buffer" },

            { "ID3D11DeviceContext::Map / Unmap",
              "vkMapMemory / vkUnmapMemory",
              "CPU-accessible memory mapping" },

            { "ID3D11Device::CreateVertexShader\n"
              "                 / CreatePixelShader",
              "vkCreateShaderModule",
              "Compile HLSL/SPIR-V shader binary into a shader module" },

            { "ID3D11DeviceContext::PSSetShaderResources",
              "vkCmdBindDescriptorSets",
              "Bind textures / buffers to the pipeline" },

            { "IDXGISwapChain::Present",
              "vkQueuePresentKHR",
              "Present the rendered frame to the display" },

            { "ID3D11RenderTargetView",
              "VkImageView (color attachment)",
              "View of a texture used as a render target" },

            { "ID3D11DepthStencilView",
              "VkImageView (depth attachment)",
              "View of a texture used as a depth/stencil buffer" },

            { "ID3D11InputLayout",
              "VkPipelineVertexInputStateCreateInfo",
              "Describe the per-vertex attribute layout" },

            { "ID3D11RasterizerState",
              "VkPipelineRasterizationStateCreateInfo",
              "Configure fill mode, cull mode, depth bias, etc." },

            { "ID3D11BlendState",
              "VkPipelineColorBlendStateCreateInfo",
              "Alpha blending configuration" },

            { "ID3D11DepthStencilState",
              "VkPipelineDepthStencilStateCreateInfo",
              "Depth test / stencil operation setup" },

            { "ID3D11SamplerState",
              "VkSampler",
              "Texture sampling (filtering, wrap mode, anisotropy)" },
        };
    }

    void PrintMappings() const {
        std::cout << ANSI::BOLD << ANSI::CYAN
                  << "\n┌──────────────────────────────────────────────────────────────────┐\n"
                  << "│         DirectX 11  →  Vulkan  API Translation Mappings         │\n"
                  << "└──────────────────────────────────────────────────────────────────┘"
                  << ANSI::RESET << "\n\n";

        for (std::size_t i = 0; i < mappings_.size(); ++i) {
            const auto& m = mappings_[i];
            std::cout << ANSI::YELLOW << "  [" << (i + 1) << "] " << ANSI::RESET
                      << ANSI::BOLD   << m.dx11Function    << ANSI::RESET << "\n"
                      << "       → " << ANSI::GREEN << m.vulkanFunction << ANSI::RESET << "\n"
                      << "       ℹ  " << m.description     << "\n\n";
        }
    }

private:
    std::vector<TranslationEntry> mappings_;
};

// ─────────────────────────────────────────────────────────────────────────────
// Vulkan Renderer
// ─────────────────────────────────────────────────────────────────────────────

#ifdef VULKAN_SDK_AVAILABLE

class VulkanRenderer {
public:
    VulkanRenderer() = default;
    ~VulkanRenderer() { Shutdown(); }

    // Non-copyable
    VulkanRenderer(const VulkanRenderer&)            = delete;
    VulkanRenderer& operator=(const VulkanRenderer&) = delete;

    bool Initialize() {
        try {
            CreateInstance();
            SelectPhysicalDevice();
            CreateLogicalDevice();
            initialized_ = true;
            return true;
        } catch (const std::exception& ex) {
            std::cerr << ANSI::RED << "[VulkanRenderer] Initialization failed: "
                      << ex.what() << ANSI::RESET << "\n";
            return false;
        }
    }

    void RenderFrame(uint32_t frameIndex) {
        if (!initialized_) return;
        std::cout << ANSI::GREEN << "  [Frame " << frameIndex << "] "
                  << ANSI::RESET << "Recording command buffer… "
                  << "vkBeginCommandBuffer → render pass → vkEndCommandBuffer → "
                  << "vkQueueSubmit → vkQueuePresentKHR\n";
    }

    void Shutdown() {
        if (!initialized_) return;
        initialized_ = false;

        if (device_ != VK_NULL_HANDLE) {
            vkDestroyDevice(device_, nullptr);
            device_ = VK_NULL_HANDLE;
        }
        if (instance_ != VK_NULL_HANDLE) {
            vkDestroyInstance(instance_, nullptr);
            instance_ = VK_NULL_HANDLE;
        }
        std::cout << ANSI::CYAN << "[VulkanRenderer] Resources released.\n" << ANSI::RESET;
    }

    const std::string& GetGpuName()      const { return gpuName_; }
    bool               IsInitialized()   const { return initialized_; }

private:
    // ── Validation layers ──────────────────────────────────────────────────
    static constexpr const char* kValidationLayer = "VK_LAYER_KHRONOS_validation";

    bool ValidationLayerAvailable() const {
        uint32_t count = 0;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        std::vector<VkLayerProperties> layers(count);
        vkEnumerateInstanceLayerProperties(&count, layers.data());
        for (const auto& l : layers)
            if (std::strcmp(l.layerName, kValidationLayer) == 0)
                return true;
        return false;
    }

    // ── Instance ───────────────────────────────────────────────────────────
    void CreateInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = "NRE Vulkan Translation Demo";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        appInfo.pEngineName        = "Nature Reality Engine";
        appInfo.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
        appInfo.apiVersion         = VK_API_VERSION_1_2;

        // Required instance extensions for WSL / Linux
        std::vector<const char*> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(PLATFORM_LINUX)
            // Prefer xcb; fall back to wayland if xcb unavailable at runtime.
            // For headless / WSL without a display server we skip surface
            // creation entirely — still useful for compute / offline rendering.
            "VK_KHR_xcb_surface",   // X11 via XCB (most common under WSLg)
#endif
        };

        std::vector<const char*> validationLayers;
#ifndef NDEBUG
        if (ValidationLayerAvailable()) {
            validationLayers.push_back(kValidationLayer);
            std::cout << ANSI::YELLOW << "[VulkanRenderer] Validation layers ENABLED\n"
                      << ANSI::RESET;
        } else {
            std::cout << ANSI::YELLOW
                      << "[VulkanRenderer] Validation layer not available — "
                         "install vulkan-validationlayers for full debugging.\n"
                      << ANSI::RESET;
        }
#endif

        VkInstanceCreateInfo createInfo{};
        createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo        = &appInfo;
        createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        createInfo.enabledLayerCount       = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames     = validationLayers.data();

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);
        if (result != VK_SUCCESS) {
            // Surface extensions might not be available in headless WSL — retry
            // without them so we can still demonstrate device enumeration.
            if (result == VK_ERROR_EXTENSION_NOT_PRESENT && !extensions.empty()) {
                std::cout << ANSI::YELLOW
                          << "[VulkanRenderer] xcb surface extension unavailable "
                             "(headless WSL?). Retrying without surface extensions.\n"
                          << ANSI::RESET;
                extensions.clear();
                createInfo.enabledExtensionCount   = 0;
                createInfo.ppEnabledExtensionNames = nullptr;
                result = vkCreateInstance(&createInfo, nullptr, &instance_);
            }
            if (result != VK_SUCCESS) {
                throw std::runtime_error(
                    "vkCreateInstance failed (VkResult=" + std::to_string(result) + ")");
            }
        }

        std::cout << ANSI::GREEN << "[VulkanRenderer] Instance created.\n" << ANSI::RESET;
    }

    // ── Physical device ────────────────────────────────────────────────────
    void SelectPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
        if (deviceCount == 0)
            throw std::runtime_error("No Vulkan-capable GPU found.");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

        // Prefer discrete GPU; otherwise take the first available device.
        for (const auto& dev : devices) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(dev, &props);
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                physicalDevice_ = dev;
                gpuName_        = props.deviceName;
                break;
            }
        }
        if (physicalDevice_ == VK_NULL_HANDLE) {
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(devices[0], &props);
            physicalDevice_ = devices[0];
            gpuName_        = props.deviceName;
        }

        std::cout << ANSI::GREEN << "[VulkanRenderer] GPU selected: "
                  << ANSI::BOLD << gpuName_ << ANSI::RESET << "\n";

        // Report ray-tracing capability
#ifdef ENABLE_RAY_TRACING
        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> exts(extCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extCount, exts.data());

        bool hasRayTracing = false;
        for (const auto& e : exts)
            if (std::strcmp(e.extensionName, VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) == 0)
                { hasRayTracing = true; break; }

        std::cout << ANSI::CYAN << "[VulkanRenderer] Ray tracing: "
                  << (hasRayTracing ? "✓ supported" : "✗ not supported on this device")
                  << ANSI::RESET << "\n";
#endif
    }

    // ── Logical device + queue ─────────────────────────────────────────────
    void CreateLogicalDevice() {
        // Find a graphics-capable queue family.
        uint32_t qfCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &qfCount, nullptr);
        std::vector<VkQueueFamilyProperties> qfProps(qfCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &qfCount, qfProps.data());

        std::optional<uint32_t> graphicsFamily;
        for (uint32_t i = 0; i < qfCount; ++i) {
            if (qfProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsFamily = i;
                break;
            }
        }
        if (!graphicsFamily)
            throw std::runtime_error("No graphics queue family found.");

        float priority = 1.0f;
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = *graphicsFamily;
        queueInfo.queueCount       = 1;
        queueInfo.pQueuePriorities = &priority;

        // Optional ray-tracing device extensions
        std::vector<const char*> deviceExtensions;
#ifdef ENABLE_RAY_TRACING
        // These are added speculatively; if the device doesn't expose them the
        // logical-device creation still succeeds — we just won't enable RT.
        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extCount, nullptr);
        std::vector<VkExtensionProperties> available(extCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extCount, available.data());

        auto hasExt = [&](const char* name) {
            for (const auto& e : available)
                if (std::strcmp(e.extensionName, name) == 0) return true;
            return false;
        };

        if (hasExt(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME)) {
            deviceExtensions.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
            deviceExtensions.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
            deviceExtensions.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
        }
#endif

        VkPhysicalDeviceFeatures features{};
        VkDeviceCreateInfo deviceInfo{};
        deviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.queueCreateInfoCount    = 1;
        deviceInfo.pQueueCreateInfos       = &queueInfo;
        deviceInfo.pEnabledFeatures        = &features;
        deviceInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
        deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

        VkResult result = vkCreateDevice(physicalDevice_, &deviceInfo, nullptr, &device_);
        if (result != VK_SUCCESS)
            throw std::runtime_error(
                "vkCreateDevice failed (VkResult=" + std::to_string(result) + ")");

        vkGetDeviceQueue(device_, *graphicsFamily, 0, &graphicsQueue_);
        std::cout << ANSI::GREEN << "[VulkanRenderer] Logical device + graphics queue created.\n"
                  << ANSI::RESET;
    }

    // ── State ──────────────────────────────────────────────────────────────
    VkInstance       instance_       = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    VkDevice         device_         = VK_NULL_HANDLE;
    VkQueue          graphicsQueue_  = VK_NULL_HANDLE;
    std::string      gpuName_;
    bool             initialized_    = false;
};

#else  // VULKAN_SDK_AVAILABLE not defined ────────────────────────────────────

// Stub renderer used when Vulkan SDK is unavailable at compile time.
class VulkanRenderer {
public:
    bool Initialize() {
        std::cout << ANSI::YELLOW
                  << "[VulkanRenderer] Built without Vulkan SDK — "
                     "running in stub mode.\n"
                  << ANSI::RESET;
        return false;
    }
    void RenderFrame(uint32_t) {}
    void Shutdown() {}
    bool        IsInitialized() const { return false; }
    std::string GetGpuName()    const { return "N/A (stub)"; }
};

#endif  // VULKAN_SDK_AVAILABLE

}  // namespace NRE

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    // ── Banner ──────────────────────────────────────────────────────────────
    std::cout << ANSI::BOLD << ANSI::CYAN
              << "\n╔══════════════════════════════════════════════════════════════════╗\n"
              << "║   Nature Reality Engine — Vulkan Renderer + API Translation Demo  ║\n"
              << "║   DirectX 11 → Vulkan   |   WSL / Linux compatible               ║\n"
              << "╚══════════════════════════════════════════════════════════════════╝\n"
              << ANSI::RESET << "\n";

    // ── 1. Print the DX11 → Vulkan translation table ─────────────────────────
    NRE::DX11ToVulkanTranslator translator;
    translator.PrintMappings();

    // ── 2. Vulkan renderer initialisation ────────────────────────────────────
    std::cout << ANSI::BOLD << ANSI::CYAN
              << "┌──────────────────────────────────────────────────────────────────┐\n"
              << "│                  Vulkan Renderer Initialization                  │\n"
              << "└──────────────────────────────────────────────────────────────────┘\n"
              << ANSI::RESET;

    NRE::VulkanRenderer renderer;
    const bool ok = renderer.Initialize();

    if (!ok) {
        std::cout << ANSI::YELLOW
                  << "\n[Demo] Vulkan initialization unsuccessful.\n"
                  << "       Common causes on WSL:\n"
                  << "         • libvulkan not installed  →  sudo apt install libvulkan1\n"
                  << "         • No GPU driver / Mesa ICD →  sudo apt install mesa-vulkan-drivers\n"
                  << "         • Missing WSLg support     →  upgrade to WSL2 + Windows 11\n"
                  << "         • DISPLAY not set          →  export DISPLAY=:0\n"
                  << "\n"
                  << "       The translation mapping table above is still fully valid\n"
                  << "       and serves as the educational core of this demo.\n"
                  << ANSI::RESET << "\n";
        return 0;
    }

    std::cout << "\n" << ANSI::GREEN
              << "  GPU : " << renderer.GetGpuName() << "\n"
              << ANSI::RESET;

    // ── 3. Simulated render loop ──────────────────────────────────────────────
    std::cout << "\n" << ANSI::BOLD << ANSI::CYAN
              << "┌──────────────────────────────────────────────────────────────────┐\n"
              << "│                      Simulated Render Loop                       │\n"
              << "└──────────────────────────────────────────────────────────────────┘\n"
              << ANSI::RESET;

    constexpr uint32_t kFrameCount = 5;
    for (uint32_t f = 0; f < kFrameCount; ++f)
        renderer.RenderFrame(f);

    // ── 4. Clean shutdown ─────────────────────────────────────────────────────
    std::cout << "\n" << ANSI::BOLD << ANSI::CYAN
              << "┌──────────────────────────────────────────────────────────────────┐\n"
              << "│                          Shutdown                                │\n"
              << "└──────────────────────────────────────────────────────────────────┘\n"
              << ANSI::RESET;

    renderer.Shutdown();

    std::cout << "\n" << ANSI::BOLD << ANSI::GREEN
              << "  Demo completed successfully. ✓\n"
              << ANSI::RESET << "\n";
    return 0;
}
