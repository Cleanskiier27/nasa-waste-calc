# Artemis I Mission Simulation Demo

## Overview

This example simulates the complete **Artemis I** mission from launch to splashdown,
using real NASA historical data from the flight that ran
**November 16 – December 11, 2022**.

Artemis I was the first integrated test of NASA's Space Launch System (SLS) and
the Orion spacecraft.  It flew uncrewed to validate all systems before the first
crewed lunar flyby (Artemis II) and eventual lunar landing (Artemis III).

The demo narrates the mission in three acts:

| Act | Phases | Description |
|-----|--------|-------------|
| **Phase 1 – Launch: The Core** | 1–2 | SLS launch, ascent, core-stage separation |
| **Phase 2 – To the Light of Beyond** | 3–7 | TLI, transit, lunar flyby, DRO, TEI |
| **Phase 3 – Return: Down With Expected Splash** | 8–12 | Inbound transit, entry, splashdown |

---

## Real Artemis I Data

| Parameter | Value |
|-----------|-------|
| Launch | November 16, 2022 – 01:47 EST from KSC Pad 39B |
| Splashdown | December 11, 2022 – 12:40 EST |
| Total duration | 25 days, 10 h, 53 min |
| Total distance | ~1.4 million miles (2.25 million km) |
| Max distance from Earth | 432,210 km (268,553 mi) – farther than any crewed-rated spacecraft |
| Closest lunar approach | 130 km above the surface |
| Peak entry speed | 24,500 mph (Mach 32) |
| Peak heat-shield temp | ~2,760 °C (5,000 °F) |
| Splashdown location | Pacific Ocean – 33°N 118°W (off Baja California) |
| Recovery ship | USS Portland (LPD-27) |

---

## Mission Phases

1. **Launch & Ascent** – SLS lifts off; solid boosters separate; MECO at T+8 min
2. **Core Stage Separation** – ICPS takes over; parking orbit at 185 km
3. **Trans-Lunar Injection (TLI)** – 18-minute ICPS burn to 10.4 km/s
4. **Outbound Transit to Moon** – 4-day coast with mid-course corrections
5. **Lunar Flyby** – 130 km closest approach; Outbound Powered Flyby burn
6. **Distant Retrograde Orbit (DRO)** – 6-day orbit; max distance 432,210 km
7. **Trans-Earth Injection (TEI)** – ESM burn to leave DRO and target Earth
8. **Inbound Transit to Earth** – 4-day coast with trajectory corrections
9. **Service Module Separation** – ESM jettisoned 40 min before splashdown
10. **Atmospheric Entry** – Skip-entry at Mach 32; heat shield tested
11. **Parachute Deployment** – Drogue chutes at 7.6 km; three 35-m mains at 2.7 km
12. **SPLASHDOWN** – Pacific Ocean; USS Portland recovers Orion

---

## How to Build and Run

### Prerequisites

- CMake 3.20+
- C++20-capable compiler (GCC 11+, Clang 13+, MSVC 2022+)

### Build

```bash
# From the repository root
mkdir build && cd build
cmake .. -DBUILD_EXAMPLES=ON
cmake --build . --target artemis_mission_demo
```

### Run

```bash
./examples/artemis_mission_demo/artemis_mission_demo
```

### Expected Output

```
╔══════════════════════════════════════════════════════════════════╗
║           🚀  ARTEMIS I MISSION SIMULATION DEMO  🌙             ║
║                                                                  ║
║  From Core  →  To the Light of Beyond  →  Down with Expected    ║
║                          Splash                                  ║
║  Launch:      November 16, 2022 – 01:47 EST                     ║
║  Splashdown:  December 11, 2022 – 12:40 EST                     ║
╚══════════════════════════════════════════════════════════════════╝

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Phase 1 / 12 :  Launch & Ascent
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  ...
  Progress: [████████████████████████████████████████] 100%
  ✅ Phase complete.
...
╔══════════════════════════════════════════════════════════════════╗
║              🎯  MISSION COMPLETE – ARTEMIS I  🎯               ║
╚══════════════════════════════════════════════════════════════════╝
```

---

## Integration with Other Systems

### NetworkBuster NLRS (Lunar Recycling System)

Source: [github.com/Cleanskiier27/Final](https://github.com/Cleanskiier27/Final)

The demo calls `integrateWithLunarRecycling()` to simulate connecting the
Artemis I trajectory data to the NetworkBuster NLRS:

- Exports DRO orbit parameters to the resource-depot planner
- Validates the NLRS operating temperature range against lunar surface conditions
- Confirms readiness for Artemis III surface operations

### Artemis R Navigation System

The demo references the Artemis R Navigation system, which consumes the same
trajectory data produced by this simulation for real-time guidance calculations.

### Nature Reality Engine (NRE)

This example is built against the `NatureRealityEngine` interface target defined
in the root `CMakeLists.txt`.  In a full implementation the rendering subsystem
would visualise the mission trajectory in 3-D.

---

## Data Sources

- NASA Artemis I Press Kit (November 2022)
- NASA Mission Elapsed Time records
- NASA GSFC Artemis I trajectory summary
- NASA Orion Program documentation
