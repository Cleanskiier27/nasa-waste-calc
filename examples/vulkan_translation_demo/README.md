# Vulkan Translation Demo

> **Nature Reality Engine** — DirectX 11 → Vulkan API Translation Example  
> Designed for **WSL (Windows Subsystem for Linux)**, bare-metal Linux, and macOS.

---

## What This Demo Demonstrates

1. **Vulkan initialization** — instance, physical device selection, logical device, and graphics queue creation, following best practices for WSL/Linux.
2. **DirectX 11 → Vulkan translation table** — a printed mapping of 15+ common DX11 API calls and their Vulkan equivalents (educational reference).
3. **Ray-tracing capability detection** — queries the selected GPU for `VK_KHR_ray_tracing_pipeline` support.
4. **Simulated render loop** — shows the sequence of Vulkan commands used to render and present frames.
5. **Graceful headless fallback** — if no display server is available (WSL without WSLg), the translation table is still printed and the process exits cleanly.

---

## Prerequisites (WSL / Ubuntu)

### 1. Vulkan Runtime and Headers

```bash
sudo apt update
sudo apt install vulkan-tools libvulkan-dev libvulkan1
```

### 2. GPU Driver / Vulkan ICD

| GPU          | Command                                          |
|--------------|--------------------------------------------------|
| Intel / Mesa | `sudo apt install mesa-vulkan-drivers`           |
| NVIDIA       | Install the NVIDIA driver for WSL2 from [nvidia.com/Download](https://www.nvidia.com/Download/index.aspx) |
| AMD          | `sudo apt install mesa-vulkan-drivers` (RADV)   |

Verify Vulkan works:

```bash
vulkaninfo --summary
# or
vkcube   # requires a display server
```

### 3. WSLg (Graphics in WSL2)

WSLg ships with **Windows 11 + WSL2** and provides an X11/Wayland server automatically.  
If you are on Windows 10, upgrade to WSL2 and enable WSLg:

```powershell
# PowerShell (Admin)
wsl --update
wsl --install   # if not already installed
```

### 4. Validation Layers (optional, recommended for development)

```bash
sudo apt install vulkan-validationlayers
```

---

## Build Instructions

### From the repository root

```bash
# Configure (Vulkan is ON by default)
cmake -B build -DUSE_VULKAN=ON -DBUILD_EXAMPLES=ON -DENABLE_RAY_TRACING=ON

# Build
cmake --build build --target vulkan_translation_demo -j$(nproc)

# Run
./build/examples/vulkan_translation_demo/vulkan_translation_demo
```

### Headless / no display server

The demo automatically falls back to headless mode when the xcb surface extension is unavailable, so you can still see the translation table and (if a GPU driver is present) enumerate devices:

```bash
DISPLAY="" ./build/examples/vulkan_translation_demo/vulkan_translation_demo
```

---

## Expected Output

```
╔══════════════════════════════════════════════════════════════════╗
║   Nature Reality Engine — Vulkan Renderer + API Translation Demo  ║
║   DirectX 11 → Vulkan   |   WSL / Linux compatible               ║
╚══════════════════════════════════════════════════════════════════╝

┌──────────────────────────────────────────────────────────────────┐
│         DirectX 11  →  Vulkan  API Translation Mappings         │
└──────────────────────────────────────────────────────────────────┘

  [1] ID3D11Device::CreateTexture2D
       → vkCreateImage
       ℹ  Allocate a 2-D texture / render target

  [2] ID3D11DeviceContext::Draw
       → vkCmdDraw
       ℹ  Non-indexed draw call
  ...

┌──────────────────────────────────────────────────────────────────┐
│                  Vulkan Renderer Initialization                  │
└──────────────────────────────────────────────────────────────────┘
[VulkanRenderer] Instance created.
[VulkanRenderer] GPU selected: NVIDIA GeForce RTX 3080
[VulkanRenderer] Ray tracing: ✓ supported
[VulkanRenderer] Logical device + graphics queue created.

  GPU : NVIDIA GeForce RTX 3080

┌──────────────────────────────────────────────────────────────────┐
│                      Simulated Render Loop                       │
└──────────────────────────────────────────────────────────────────┘
  [Frame 0] Recording command buffer… vkBeginCommandBuffer → render pass → …
  [Frame 1] Recording command buffer… …
  [Frame 2] Recording command buffer… …
  [Frame 3] Recording command buffer… …
  [Frame 4] Recording command buffer… …

  Demo completed successfully. ✓
```

---

## Troubleshooting

| Symptom | Fix |
|---------|-----|
| `vkCreateInstance` returns `VK_ERROR_INCOMPATIBLE_DRIVER` | Install a Vulkan ICD (`mesa-vulkan-drivers` or NVIDIA WSL driver) |
| `xcb surface extension unavailable` | Expected on WSL without WSLg — demo continues in headless mode |
| `No Vulkan-capable GPU found` | Ensure your GPU driver is installed and `vulkaninfo` shows at least one device |
| Black screen / crash in `vkcube` | WSLg may not be active — run `wsl --update` from PowerShell |
| `DISPLAY not set` error | `export DISPLAY=:0` or rely on WSLg automatic DISPLAY |
| Validation layer not found | `sudo apt install vulkan-validationlayers` |

---

## Code Structure

```
examples/vulkan_translation_demo/
├── main.cpp          # VulkanRenderer + DX11ToVulkanTranslator classes + main()
├── CMakeLists.txt    # Build configuration (auto-detects Vulkan SDK)
└── README.md         # This file
```

---

## Further Reading

- [Vulkan Tutorial](https://vulkan-tutorial.com)
- [WSLg GitHub repository](https://github.com/microsoft/wslg)
- [LunarG Vulkan SDK for Linux](https://vulkan.lunarg.com/sdk/home#linux)
- [Mesa Vulkan drivers (RADV / ANV)](https://docs.mesa3d.org/drivers/index.html)
