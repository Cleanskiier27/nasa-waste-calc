# 🧭 Artemis Navigation Tracking

## Mission Overview
This document tracks the navigational systems and demo execution for the Artemis moonshot project.

## File Structure
```
examples/artemis_demo/
├── main.cpp              # L-System Tree Generator Demo
├── NAVIGATION.md         # This file - Navigation tracking
└── TRACKING.md           # Mission tracking and status
```

## Navigation Points

### 🎯 Primary Target
- **Demo File**: `main.cpp`
- **URL**: https://github.com/Cleanskiier27/nasa-waste-calc/blob/main/examples/artemis_demo/main.cpp
- **Purpose**: Demonstrate L-system procedural tree generation

### 🌲 Tree Species Supported
1. **Oak** - Angle: 25.7°, Length Factor: 0.7
2. **Pine** - Angle: 20.0°, Length Factor: 0.8
3. **Willow** - Angle: 22.5°, Length Factor: 0.6

### 📊 L-System Rules
- `F` = Draw forward (create branch segment)
- `+` = Rotate right
- `-` = Rotate left
- `[` = Push state (save position for branch)
- `]` = Pop state (return to saved position)

## Build Instructions

### Compile and Run
```bash
cd examples/artemis_demo
g++ -std=c++17 main.cpp -o artemis_demo
./artemis_demo
```

### Expected Output
- ASCII tree visualizations for 3 species
- Branch statistics
- L-system generation metrics

## Integration with Nature Reality Engine

This demo showcases core technology that powers:
- 🌲 Millions of branches with physics
- 🍃 Individual leaves with wind simulation
- 🍂 Seasonal color changes
- 📸 Ray-traced bark textures at 8K

## Next Navigation Waypoints
- [ ] Add CMakeLists.txt for artemis_demo
- [ ] Integrate with main engine renderer
- [ ] Add wind physics simulation
- [ ] Implement photorealistic textures

---
**Last Updated**: 2026-03-02
**Status**: ✅ Operational