"""
Pydantic models for Artemis II Mission Control System
"""
from datetime import datetime
from typing import List, Optional, Dict, Any
from pydantic import BaseModel, Field


class Vector3D(BaseModel):
    """3D vector for position and velocity"""
    x: float = Field(..., description="X coordinate")
    y: float = Field(..., description="Y coordinate")
    z: float = Field(..., description="Z coordinate")


class OrbitalParameters(BaseModel):
    """Orbital parameters"""
    apogee: float = Field(..., description="Highest point (km)")
    perigee: float = Field(..., description="Lowest point (km)")
    inclination: float = Field(..., description="Orbital inclination (degrees)")
    period: float = Field(..., description="Orbital period (seconds)")


class SpacecraftTelemetry(BaseModel):
    """Real-time spacecraft telemetry data"""
    position: Vector3D = Field(..., description="Position vector (km)")
    velocity: Vector3D = Field(..., description="Velocity vector (km/s)")
    altitude: float = Field(..., description="Altitude from Earth (km)")
    distance_earth: float = Field(..., description="Distance from Earth center (km)")
    distance_moon: float = Field(..., description="Distance from Moon center (km)")
    mission_elapsed_time: str = Field(..., description="Days:Hours:Minutes:Seconds")
    orbital_parameters: OrbitalParameters
    timestamp: datetime = Field(default_factory=datetime.utcnow)


class Vitals(BaseModel):
    """Crew member vital signs"""
    heart_rate: int = Field(..., description="BPM (60-100 nominal)", ge=0, le=250)
    blood_pressure: str = Field(..., description="Format: 120/80")
    oxygen_saturation: int = Field(..., description="SpO2 percentage (95-100%)", ge=0, le=100)
    temperature: float = Field(..., description="°C (36.5-37.5°C nominal)")
    respiratory_rate: int = Field(..., description="Breaths per minute", ge=0, le=60)


class Alert(BaseModel):
    """Medical or system alert"""
    severity: str = Field(..., description="INFO, WARNING, CAUTION, CRITICAL")
    system: str = Field(..., description="System name")
    message: str
    timestamp: datetime = Field(default_factory=datetime.utcnow)
    acknowledged: bool = False
    resolution: Optional[str] = None


class CrewMember(BaseModel):
    """Crew member health data"""
    name: str
    role: str = Field(..., description="Commander, Pilot, Mission Specialist")
    vitals: Vitals
    radiation_exposure: float = Field(..., description="mSv cumulative")
    sleep_hours_last_24h: float = Field(..., description="Hours of sleep")
    exercise_compliance: bool = Field(..., description="Daily exercise completed")
    medical_alerts: List[Alert] = Field(default_factory=list)


class OxygenSystem(BaseModel):
    """Oxygen generation and management"""
    generation_rate: float = Field(..., description="kg/hour")
    tank_pressure: float = Field(..., description="PSI")
    partial_pressure: float = Field(..., description="mmHg (160 nominal)")
    consumption_rate: float = Field(..., description="kg/hour (crew of 4)")


class CO2Removal(BaseModel):
    """CO2 removal system"""
    scrubber_status: str = Field(..., description="Active, Standby, Degraded")
    co2_level: float = Field(..., description="mmHg (<4.0 mmHg required)")
    lioh_remaining: float = Field(..., description="kg of LiOH canisters")


class WaterSystem(BaseModel):
    """Water management"""
    potable_water: float = Field(..., description="Liters remaining")
    wastewater: float = Field(..., description="Liters to process")
    humidity: float = Field(..., description="Percentage (30-70%)")


class CabinEnvironment(BaseModel):
    """Cabin environmental conditions"""
    pressure: float = Field(..., description="PSI (14.7 nominal)")
    temperature: float = Field(..., description="°C (18-27°C range)")
    air_quality_index: int = Field(..., description="0-100 (100 = excellent)", ge=0, le=100)


class ECLSS(BaseModel):
    """Environmental Control and Life Support System"""
    oxygen: OxygenSystem
    co2_removal: CO2Removal
    water: WaterSystem
    cabin: CabinEnvironment
    timestamp: datetime = Field(default_factory=datetime.utcnow)


class SolarArrays(BaseModel):
    """Solar array power generation"""
    output_current: float = Field(..., description="Kilowatts")
    voltage: float = Field(..., description="Volts")
    panel_temperature: float = Field(..., description="°C")
    sun_angle: float = Field(..., description="Degrees (optimal: 90°)")
    degradation: float = Field(..., description="Percentage (age-related)")


class Battery(BaseModel):
    """Battery status"""
    id: int
    state_of_charge: float = Field(..., description="Percentage (0-100%)", ge=0, le=100)
    voltage: float = Field(..., description="Volts")
    temperature: float = Field(..., description="°C")
    health: str = Field(..., description="Good, Fair, Poor")
    cycles: int = Field(..., description="Charge/discharge cycles")


class PowerConsumption(BaseModel):
    """Power consumption breakdown"""
    total: float = Field(..., description="Kilowatts")
    by_subsystem: Dict[str, float] = Field(
        ...,
        description="Power by subsystem: life_support, avionics, communications, thermal, propulsion"
    )


class PowerBudget(BaseModel):
    """Power budget tracking"""
    available: float = Field(..., description="kW")
    required: float = Field(..., description="kW")
    margin: float = Field(..., description="Percentage")


class PowerSystem(BaseModel):
    """Complete power system status"""
    solar_arrays: SolarArrays
    batteries: List[Battery]
    power_consumption: PowerConsumption
    power_budget: PowerBudget
    timestamp: datetime = Field(default_factory=datetime.utcnow)


class MainEngine(BaseModel):
    """Main propulsion engine"""
    status: str = Field(..., description="Ready, Firing, Safed")
    thrust: float = Field(..., description="Newtons")
    specific_impulse: float = Field(..., description="Seconds (ISP)")
    temperature: float = Field(..., description="°C")
    pressure: float = Field(..., description="PSI")
    total_burn_time: float = Field(..., description="Seconds (cumulative)")


class RCSThruster(BaseModel):
    """Reaction Control System thruster"""
    thruster_id: str = Field(..., description="e.g., F1L, A2R")
    status: str = Field(..., description="Active, Failed, Disabled")
    firing_count: int = Field(..., description="Total firings")
    propellant_used: float = Field(..., description="kg")


class Fuel(BaseModel):
    """Fuel and propellant status"""
    hydrazine: float = Field(..., description="kg remaining")
    mon_oxidizer: float = Field(..., description="kg remaining (MON = Mixed Oxides of Nitrogen)")
    total_delta_v: float = Field(..., description="m/s available")
    used_delta_v: float = Field(..., description="m/s expended")


class UpcomingBurn(BaseModel):
    """Planned propulsion burn"""
    name: str = Field(..., description="TLI, LOI, TEI")
    time: datetime
    duration: float = Field(..., description="seconds")
    delta_v: float = Field(..., description="m/s")


class PropulsionSystem(BaseModel):
    """Complete propulsion system"""
    main_engine: MainEngine
    rcs_thrusters: List[RCSThruster]
    fuel: Fuel
    upcoming_burns: List[UpcomingBurn]
    timestamp: datetime = Field(default_factory=datetime.utcnow)


class DSNContact(BaseModel):
    """Deep Space Network ground station contact"""
    station: str = Field(..., description="Goldstone, Madrid, Canberra")
    status: str = Field(..., description="Active, Scheduled, Lost Signal")
    signal_strength: float = Field(..., description="dBm")
    data_rate: float = Field(..., description="Mbps")
    uplink_freq: float = Field(..., description="MHz")
    downlink_freq: float = Field(..., description="MHz")
    acquisition_time: datetime
    loss_of_signal: datetime


class CurrentDownlink(BaseModel):
    """Current downlink status"""
    video: bool = Field(..., description="Video available")
    telemetry: float = Field(..., description="Packets per second")
    voice: bool = Field(..., description="Voice loop active")
    delay: float = Field(..., description="Seconds (one-way light time)")


class Communications(BaseModel):
    """Communications system status"""
    dsn_contacts: List[DSNContact]
    current_downlink: CurrentDownlink
    message_queue: int = Field(..., description="Messages awaiting transmission")
    timestamp: datetime = Field(default_factory=datetime.utcnow)


class MissionEvent(BaseModel):
    """Mission timeline event"""
    met: str = Field(..., description="Mission Elapsed Time (T+0:00:00)")
    event: str
    status: str = Field(..., description="Complete, Upcoming, In Progress")
    description: str


class MissionTimeline(BaseModel):
    """Complete mission timeline"""
    events: List[MissionEvent]
    current_phase: str
    next_major_event: Optional[MissionEvent] = None
