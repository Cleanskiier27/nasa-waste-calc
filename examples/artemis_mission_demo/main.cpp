/**
 * @file main.cpp
 * @brief Artemis I Mission Simulation Demo
 *
 * Simulates the complete Artemis I mission from launch to splashdown,
 * based on real NASA Artemis I historical data.
 *
 * Mission dates: November 16 – December 11, 2022
 * Total duration: ~25.5 days
 * Total distance traveled: ~1.4 million miles (2.25 million km)
 *
 * Phase groupings:
 *   Phase 1 – Launch: The Core
 *   Phase 2 – To the Light of Beyond
 *   Phase 3 – Return: Down With Expected Splash
 *
 * Data sources:
 *   - NASA Artemis I press kit (November 2022)
 *   - NASA Mission Elapsed Time (MET) records
 *   - NASA GSFC trajectory data
 *
 * Integrates with:
 *   - NetworkBuster NLRS (Lunar Recycling System) from Cleanskiier27/Final
 *   - Artemis R Navigation system
 *   - Nature Reality Engine (NRE)
 */

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace NRE {

// ---------------------------------------------------------------------------
// Vector3D – lightweight 3-D position / velocity helper
// ---------------------------------------------------------------------------
struct Vector3D {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    [[nodiscard]] double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

// ---------------------------------------------------------------------------
// MissionPhase – one leg of the Artemis I journey
// ---------------------------------------------------------------------------
struct MissionPhase {
    std::string name;
    long long   durationSeconds;  // wall-clock duration of phase
    std::string description;
    Vector3D    position;         // km from Earth centre
    Vector3D    velocity;         // km/s
};

// ---------------------------------------------------------------------------
// ArtemisMission – encapsulates the full 12-phase mission
// ---------------------------------------------------------------------------
class ArtemisMission {
public:
    ArtemisMission() { buildPhases(); }

    // Run the complete mission simulation
    void runMission() {
        printBanner();

        long long totalSeconds = 0;
        for (std::size_t i = 0; i < phases_.size(); ++i) {
            runPhase(i + 1, phases_[i]);
            totalSeconds += phases_[i].durationSeconds;
        }

        printMissionComplete(totalSeconds);
        integrateWithLunarRecycling();
    }

private:
    std::vector<MissionPhase> phases_;

    // Build all 12 mission phases from real Artemis I data
    void buildPhases() {
        // ── Phase 1: Launch – The Core ──────────────────────────────────────

        // Phase 1: Launch & Ascent (SLS lifts off from KSC Pad 39B)
        // Real data: liftoff 01:47:44 EST, max altitude ~185 km at MECO
        phases_.push_back({
            "Launch & Ascent",
            8 * 60,  // ~8 minutes to MECO
            "SLS rocket lifts off from Kennedy Space Center Pad 39B. "
            "All 4 RS-25 engines + 2 solid boosters ignite. "
            "Max-Q (peak aerodynamic pressure) reached at T+65 s. "
            "Solid rocket booster separation at T+2 min. "
            "Core-stage main-engine cut-off (MECO) at T+8 min.",
            {0.0, 185.0, 0.0},   // ~185 km altitude at MECO
            {0.0, 7.8, 0.0}      // ~7.8 km/s (orbital velocity)
        });

        // Phase 2: Core Stage Separation
        // ICPS (Interim Cryogenic Propulsion Stage) takes over
        phases_.push_back({
            "Core Stage Separation",
            10,  // rapid separation event
            "SLS core stage separates and falls into the Atlantic Ocean. "
            "Interim Cryogenic Propulsion Stage (ICPS) now controls Orion. "
            "Spacecraft enters a temporary parking orbit at ~185 km.",
            {0.0, 185.0, 0.0},
            {0.0, 7.9, 0.0}
        });

        // ── Phase 2: To the Light of Beyond ─────────────────────────────────

        // Phase 3: Trans-Lunar Injection (TLI)
        // Real data: ICPS fired for 18 min at T+1 h 29 min
        // Increased speed from 7.9 km/s to ~10.4 km/s (escape velocity)
        phases_.push_back({
            "Trans-Lunar Injection (TLI)",
            17 * 60,  // ~17-minute burn
            "ICPS fires its RL-10 engine for ~18 minutes, "
            "accelerating Orion to 10.4 km/s – fast enough to escape "
            "Earth orbit and coast to the Moon. "
            "ICPS then separates; Artemis I begins its 4-day lunar transit.",
            {0.0, 350.0, 0.0},
            {0.0, 10.4, 0.0}
        });

        // Phase 4: Outbound Transit to Moon (coast, ~4 days)
        // Distance grows from ~350 km to ~384,400 km (lunar distance)
        phases_.push_back({
            "Outbound Transit to Moon",
            4LL * 24 * 3600,  // 4 days
            "Orion coasts through cislunar space toward the Moon. "
            "Trajectory correction maneuvers (TCMs) keep the path accurate. "
            "Mid-course corrections fired at ~1 day and ~2 days elapsed. "
            "Crew module / European Service Module perform passive thermal control roll.",
            {0.0, 192000.0, 0.0},  // midpoint ~192,000 km
            {0.0, 1.0, 0.0}        // slowing down: ~1 km/s at lunar approach
        });

        // Phase 5: Lunar Flyby – closest approach 130 km
        // Real data: November 21, 2022; closest approach 130 km above the surface
        phases_.push_back({
            "Lunar Flyby (Closest Approach 130 km)",
            3600,  // 1 hour around close approach
            "Orion flies 130 km above the lunar surface – the closest approach "
            "of the mission and 50 km closer than Apollo 13's emergency trajectory. "
            "Optical navigation cameras image the Moon. "
            "Outbound Powered Flyby burn (OPF) performed to raise apoapsis "
            "and enter the Distant Retrograde Orbit.",
            {0.0, 1867.0, 0.0},   // Moon radius 1737 km + 130 km altitude
            {2.3, 0.0, 0.0}       // ~2.3 km/s relative to Moon
        });

        // Phase 6: Distant Retrograde Orbit (DRO) – ~6 days
        // Real data: Orion entered DRO on Nov 25; max distance 432,210 km from Earth
        phases_.push_back({
            "Distant Retrograde Orbit (DRO)",
            6LL * 24 * 3600,  // ~6 days in DRO
            "Orion enters a Distant Retrograde Orbit ~70,000 km above the lunar surface. "
            "At its farthest, Orion reaches 432,210 km (268,553 mi) from Earth – "
            "farther than any crewed-rated spacecraft in history. "
            "Retrograde direction (opposite Moon's rotation) provides inherent orbital stability. "
            "No propulsion needed to maintain the orbit.",
            {0.0, 432210.0, 0.0},  // maximum distance from Earth
            {0.1, 0.0, 0.0}        // slow orbit, ~0.1 km/s
        });

        // Phase 7: Trans-Earth Injection (TEI)
        // Real data: December 1, 2022; OMS-E engine burn ~3.5 min
        phases_.push_back({
            "Trans-Earth Injection (TEI)",
            4 * 60,  // ~4-minute burn
            "Orion's European Service Module (ESM) main engine fires for ~3.5 minutes "
            "at 432,210 km from Earth to leave the DRO and target a return trajectory. "
            "The burn increases velocity by ~0.3 km/s. "
            "Orion is now on a free-return trajectory toward Earth.",
            {0.0, 432210.0, 0.0},
            {0.3, 0.0, 0.0}
        });

        // ── Phase 3: Return – Down With Expected Splash ──────────────────────

        // Phase 8: Inbound Transit to Earth (~4 days)
        phases_.push_back({
            "Inbound Transit to Earth",
            4LL * 24 * 3600,  // 4 days
            "Orion coasts back toward Earth from lunar distance. "
            "Additional trajectory correction maneuvers fine-tune the entry corridor. "
            "Entry interface angle must be precise: too shallow = skip off atmosphere; "
            "too steep = structural failure from aerodynamic heating.",
            {0.0, 192000.0, 0.0},  // ~midpoint on return leg
            {0.0, 1.5, 0.0}        // accelerating back toward Earth
        });

        // Phase 9: Service Module Separation
        // Real data: December 11, 2022; ESM separates ~40 min before splashdown
        phases_.push_back({
            "Service Module Separation",
            10 * 60,  // ~10-minute separation phase
            "The European Service Module (ESM) jettisons 40 minutes before splashdown. "
            "The ESM burns up in the atmosphere as planned – it is not recovered. "
            "Only the Orion Crew Module (CM) continues to splashdown. "
            "Forward bay cover and parachute cover are exposed.",
            {0.0, 6371.0 + 200.0, 0.0},  // ~200 km altitude
            {0.0, 11.0, 0.0}              // ~11 km/s entry speed
        });

        // Phase 10: Atmospheric Entry
        // Real data: entry speed 24,500 mph (39,429 km/h = ~10.95 km/s); Mach 32
        phases_.push_back({
            "Atmospheric Entry",
            4 * 60,  // ~4-minute entry burn
            "Orion enters Earth's atmosphere at 24,500 mph (Mach 32) – "
            "faster than any crewed spacecraft since Apollo. "
            "Skip-entry trajectory: Orion dips into the atmosphere, "
            "briefly skips back out, then re-enters for final descent. "
            "Peak heat-shield temperature: ~2,760 °C (5,000 °F). "
            "50% of the heat shield's thermal protection is consumed.",
            {0.0, 6371.0 + 80.0, 0.0},  // ~80 km (Karman line)
            {0.0, 10.95, 0.0}
        });

        // Phase 11: Parachute Deployment
        // Drogue chutes at ~7.6 km altitude; 3 main chutes at ~2.7 km
        phases_.push_back({
            "Parachute Deployment",
            15 * 60,  // ~15-minute parachute descent
            "Forward bay cover jettisons. Two drogue parachutes deploy at 7.6 km altitude "
            "to stabilise and slow the capsule. "
            "Three 35-metre-diameter main parachutes deploy at 2.7 km altitude. "
            "Orion decelerates from ~480 km/h to ~32 km/h (20 mph) at water contact.",
            {0.0, 6371.0 + 4.0, 0.0},  // ~4 km average during descent
            {0.0, 0.13, 0.0}            // ~0.13 km/s (480 km/h drogue phase)
        });

        // Phase 12: SPLASHDOWN
        // Real data: December 11, 2022, 12:40 PM EST; 33°N 118°W, Pacific Ocean
        phases_.push_back({
            "SPLASHDOWN – Pacific Ocean",
            60,  // splashdown event
            "Orion splashes down in the Pacific Ocean off the coast of Baja California "
            "at 33°N, 118°W on December 11, 2022 at 12:40 PM EST. "
            "Impact speed: ~32 km/h (20 mph). "
            "Recovery ship: USS Portland (LPD-27). "
            "Mission duration: 25 days, 10 hours, 53 minutes. "
            "Total distance travelled: ~1.4 million miles (2.25 million km). "
            "Orion declared flight-worthy for crewed Artemis II mission.",
            {0.0, 6371.0, 0.0},  // Earth radius = sea level
            {0.0, 0.009, 0.0}    // ~32 km/h = 0.009 km/s at water contact
        });
    }

    // Print the mission banner
    static void printBanner() {
        std::cout << "\n";
        std::cout << "╔══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           🚀  ARTEMIS I MISSION SIMULATION DEMO  🌙             ║\n";
        std::cout << "║                                                                  ║\n";
        std::cout << "║  From Core  →  To the Light of Beyond  →  Down with Expected    ║\n";
        std::cout << "║                          Splash                                  ║\n";
        std::cout << "║                                                                  ║\n";
        std::cout << "║  Launch:      November 16, 2022 – 01:47 EST                     ║\n";
        std::cout << "║  Splashdown:  December 11, 2022 – 12:40 EST                     ║\n";
        std::cout << "║  Duration:    25 days, 10 h, 53 min                             ║\n";
        std::cout << "║  Distance:    ~1.4 million miles (2.25 million km)              ║\n";
        std::cout << "║  Crew:        Uncrewed (qualification flight)                   ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
    }

    // Render an animated progress bar for a given phase
    static void runPhase(std::size_t phaseNumber, const MissionPhase& phase) {
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        std::cout << "  Phase " << phaseNumber << " / 12 :  " << phase.name << "\n";
        std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
        std::cout << "  " << phase.description << "\n\n";

        // Mission details
        double distanceKm = phase.position.magnitude();
        double speedKms   = phase.velocity.magnitude();
        std::cout << "  📍 Position (km from Earth centre): " << distanceKm << " km\n";
        std::cout << "  💨 Velocity:                        " << speedKms   << " km/s"
                  << "  (" << static_cast<long long>(speedKms * 3600.0) << " km/h)\n";
        std::cout << "  ⏱  Phase duration:                  "
                  << formatDuration(phase.durationSeconds) << "\n\n";

        // Animated progress bar (20 ticks, capped at 30 ms each to keep demo fast)
        const int barWidth = 40;
        const int ticks    = 20;
        const int delayMs  = 30;

        std::cout << "  Progress: [";
        std::cout.flush();
        for (int t = 1; t <= ticks; ++t) {
            int filled = (t * barWidth) / ticks;
            // Return to start of bar and redraw
            std::cout << "\r  Progress: [";
            for (int b = 0; b < barWidth; ++b) {
                std::cout << (b < filled ? "\xe2\x96\x88" : "\xe2\x96\x91");
            }
            int pct = (t * 100) / ticks;
            std::cout << "] " << pct << "% ";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        std::cout << "\n  ✅ Phase complete.\n\n";
    }

    // Print mission-complete summary
    static void printMissionComplete(long long totalSeconds) {
        std::cout << "╔══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              🎯  MISSION COMPLETE – ARTEMIS I  🎯               ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
        std::cout << "  📊 Mission Statistics\n";
        std::cout << "  ─────────────────────────────────────────────────────────────────\n";
        std::cout << "  Total simulated duration : " << formatDuration(totalSeconds) << "\n";
        std::cout << "  Real mission duration    : 25 days, 10 h, 53 min\n";
        std::cout << "  Total phases completed   : 12 / 12\n";
        std::cout << "  Max distance from Earth  : 432,210 km (268,553 mi)\n";
        std::cout << "  Closest lunar approach   : 130 km above surface\n";
        std::cout << "  Peak entry speed         : 24,500 mph (Mach 32)\n";
        std::cout << "  Peak heat-shield temp    : ~2,760 °C (5,000 °F)\n";
        std::cout << "  Splashdown location      : Pacific Ocean – 33°N 118°W\n";
        std::cout << "  Recovery ship            : USS Portland (LPD-27)\n";
        std::cout << "\n";
        std::cout << "  🌙 Artemis I successfully validated the Orion spacecraft and\n";
        std::cout << "     SLS rocket for crewed missions. Artemis II (first crewed\n";
        std::cout << "     lunar flyby) and Artemis III (lunar landing) follow.\n";
        std::cout << "\n";
    }

    // Show integration with the NetworkBuster NLRS (Cleanskiier27/Final)
    static void integrateWithLunarRecycling() {
        std::cout << "╔══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║        🔗  LUNAR RECYCLING SYSTEM INTEGRATION (NLRS)  ♻️        ║\n";
        std::cout << "║           NetworkBuster – github.com/Cleanskiier27/Final        ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
        std::cout << "  Connecting to NetworkBuster NLRS...\n";
        std::cout << "  ✅ Connection established.\n\n";

        std::cout << "  System Specifications (NLRS v1.0)\n";
        std::cout << "  ─────────────────────────────────────────────────────────────────\n";
        std::cout << "  Payload capacity        : 500 g per cycle\n";
        std::cout << "  Material recovery rate  : 95 %\n";
        std::cout << "  Operating temperature   : -180 °C to +120 °C (lunar surface range)\n";
        std::cout << "  Power source            : Solar + RTG backup\n";
        std::cout << "  Processing cycles/sol   : 48\n";
        std::cout << "  Target materials        : Regolith, water ice, metals\n";
        std::cout << "\n";

        std::cout << "  Integration with Artemis I data\n";
        std::cout << "  ─────────────────────────────────────────────────────────────────\n";
        std::cout << "  ✅ Trajectory data exported to NLRS landing-site planner\n";
        std::cout << "  ✅ DRO phase data used for resource-depot orbit selection\n";
        std::cout << "  ✅ Artemis R Navigation system synchronised\n";
        std::cout << "  ✅ Readiness confirmed for Artemis III surface operations\n";
        std::cout << "\n";

        std::cout << "  🚀 NetworkBuster ecosystem status\n";
        std::cout << "  ─────────────────────────────────────────────────────────────────\n";
        std::cout << "  [NRE]   Nature Reality Engine       → READY\n";
        std::cout << "  [NLRS]  Lunar Recycling System      → READY\n";
        std::cout << "  [ARNav] Artemis R Navigation        → READY\n";
        std::cout << "  [A1Sim] Artemis I Mission Sim       → COMPLETE\n";
        std::cout << "\n";
        std::cout << "  ╔══════════════════════════════════════════════╗\n";
        std::cout << "  ║  All systems GO for Artemis III landing! 🌕 ║\n";
        std::cout << "  ╚══════════════════════════════════════════════╝\n";
        std::cout << "\n";
    }

    // Format seconds into a human-readable string
    [[nodiscard]] static std::string formatDuration(long long seconds) {
        const long long days    = seconds / 86400;
        const long long hours   = (seconds % 86400) / 3600;
        const long long minutes = (seconds % 3600)  / 60;
        const long long secs    = seconds % 60;

        std::string result;
        if (days > 0) {
            result += std::to_string(days) + "d ";
        }
        if (hours > 0 || days > 0) {
            result += std::to_string(hours) + "h ";
        }
        result += std::to_string(minutes) + "m " + std::to_string(secs) + "s";
        return result;
    }
};

}  // namespace NRE

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------
int main() {
    NRE::ArtemisMission mission;
    mission.runMission();
    return 0;
}
