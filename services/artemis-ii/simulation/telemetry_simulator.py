"""
Telemetry simulator for Artemis II mission
Generates realistic spacecraft, crew, and systems data
"""
import random
import math
import numpy as np
from datetime import datetime, timedelta
from typing import Dict, Any
from app.models import *
from app.orbital_mechanics import OrbitalMechanics


class TelemetrySimulator:
    """Generate realistic telemetry data for Artemis II"""
    
    def __init__(self, mission_start_time: datetime = None):
        if mission_start_time is None:
            mission_start_time = datetime.utcnow()
        self.mission_start_time = mission_start_time
        self.current_time = mission_start_time
        self.orbital_mechanics = OrbitalMechanics(mission_start_time)
        self.crew_names = [
            "Reid Wiseman",
            "Victor Glover",
            "Christina Koch",
            "Jeremy Hansen"
        ]
        self.crew_roles = ["Commander", "Pilot", "Mission Specialist 1", "Mission Specialist 2"]
        
    def advance_time(self, seconds: float = 1.0):
        """Advance simulation time"""
        self.current_time += timedelta(seconds=seconds)
        self.orbital_mechanics.set_time(self.current_time)
        
    def generate_spacecraft_telemetry(self) -> SpacecraftTelemetry:
        """Generate spacecraft telemetry"""
        elapsed_seconds = (self.current_time - self.mission_start_time).total_seconds()
        
        # Get position and velocity from orbital mechanics
        position_vec, velocity_vec = self.orbital_mechanics.propagate_position(elapsed_seconds)
        altitude, distance_earth, distance_moon = self.orbital_mechanics.calculate_distances(position_vec)
        
        # Add small random variations
        position_vec += np.random.normal(0, 0.1, 3)
        velocity_vec += np.random.normal(0, 0.001, 3)
        
        position = Vector3D(x=position_vec[0], y=position_vec[1], z=position_vec[2])
        velocity = Vector3D(x=velocity_vec[0], y=velocity_vec[1], z=velocity_vec[2])
        
        # Calculate orbital parameters
        orbital_params_dict = self.orbital_mechanics.calculate_orbital_parameters(position_vec, velocity_vec)
        orbital_params = OrbitalParameters(**orbital_params_dict)
        
        met = self.orbital_mechanics.get_mission_elapsed_time()
        
        return SpacecraftTelemetry(
            position=position,
            velocity=velocity,
            altitude=altitude,
            distance_earth=distance_earth,
            distance_moon=distance_moon,
            mission_elapsed_time=met,
            orbital_parameters=orbital_params,
            timestamp=self.current_time
        )
    
    def generate_crew_member(self, index: int) -> CrewMember:
        """Generate crew member health data"""
        # Nominal values with small variations
        heart_rate = random.randint(65, 85)
        systolic = random.randint(110, 130)
        diastolic = random.randint(70, 85)
        blood_pressure = f"{systolic}/{diastolic}"
        oxygen_saturation = random.randint(96, 100)
        temperature = random.uniform(36.5, 37.3)
        respiratory_rate = random.randint(12, 18)
        
        vitals = Vitals(
            heart_rate=heart_rate,
            blood_pressure=blood_pressure,
            oxygen_saturation=oxygen_saturation,
            temperature=temperature,
            respiratory_rate=respiratory_rate
        )
        
        # Cumulative radiation exposure (increases over time)
        elapsed_days = (self.current_time - self.mission_start_time).total_seconds() / 86400.0
        radiation_exposure = elapsed_days * 0.5  # ~0.5 mSv per day
        
        sleep_hours = random.uniform(6.5, 8.5)
        exercise_compliance = random.random() > 0.2  # 80% compliance
        
        return CrewMember(
            name=self.crew_names[index],
            role=self.crew_roles[index],
            vitals=vitals,
            radiation_exposure=radiation_exposure,
            sleep_hours_last_24h=sleep_hours,
            exercise_compliance=exercise_compliance,
            medical_alerts=[]
        )
    
    def generate_eclss(self) -> ECLSS:
        """Generate ECLSS data"""
        oxygen = OxygenSystem(
            generation_rate=random.uniform(0.8, 1.2),
            tank_pressure=random.uniform(850, 900),
            partial_pressure=random.uniform(158, 162),
            consumption_rate=random.uniform(0.9, 1.1)
        )
        
        co2_removal = CO2Removal(
            scrubber_status=random.choice(["Active", "Active", "Active", "Standby"]),
            co2_level=random.uniform(2.0, 3.5),
            lioh_remaining=random.uniform(15, 25)
        )
        
        water = WaterSystem(
            potable_water=random.uniform(180, 220),
            wastewater=random.uniform(5, 15),
            humidity=random.uniform(35, 55)
        )
        
        cabin = CabinEnvironment(
            pressure=random.uniform(14.5, 14.9),
            temperature=random.uniform(20, 24),
            air_quality_index=random.randint(90, 100)
        )
        
        return ECLSS(
            oxygen=oxygen,
            co2_removal=co2_removal,
            water=water,
            cabin=cabin,
            timestamp=self.current_time
        )
    
    def generate_power_system(self) -> PowerSystem:
        """Generate power system data"""
        # Solar array output varies with sun angle
        sun_angle = random.uniform(70, 90)
        output_current = 11.0 * (sun_angle / 90.0) + random.uniform(-0.5, 0.5)
        
        solar_arrays = SolarArrays(
            output_current=output_current,
            voltage=random.uniform(115, 125),
            panel_temperature=random.uniform(-50, 50),
            sun_angle=sun_angle,
            degradation=random.uniform(0, 2)
        )
        
        batteries = []
        for i in range(4):
            batteries.append(Battery(
                id=i + 1,
                state_of_charge=random.uniform(85, 95),
                voltage=random.uniform(30, 32),
                temperature=random.uniform(15, 25),
                health=random.choice(["Good", "Good", "Good", "Fair"]),
                cycles=random.randint(50, 150)
            ))
        
        power_consumption = PowerConsumption(
            total=random.uniform(8.5, 10.5),
            by_subsystem={
                "life_support": random.uniform(2.5, 3.5),
                "avionics": random.uniform(1.5, 2.0),
                "communications": random.uniform(1.0, 1.5),
                "thermal": random.uniform(2.0, 2.5),
                "propulsion": random.uniform(1.0, 1.5)
            }
        )
        
        available = output_current
        required = power_consumption.total
        margin = ((available - required) / required) * 100
        
        power_budget = PowerBudget(
            available=available,
            required=required,
            margin=margin
        )
        
        return PowerSystem(
            solar_arrays=solar_arrays,
            batteries=batteries,
            power_consumption=power_consumption,
            power_budget=power_budget,
            timestamp=self.current_time
        )
    
    def generate_propulsion_system(self) -> PropulsionSystem:
        """Generate propulsion system data"""
        main_engine = MainEngine(
            status="Ready",
            thrust=0.0,
            specific_impulse=315.0,
            temperature=random.uniform(20, 30),
            pressure=random.uniform(100, 120),
            total_burn_time=random.uniform(200, 300)
        )
        
        # Generate 24 RCS thrusters
        rcs_thrusters = []
        positions = ["F", "A", "L", "R"]
        for i in range(24):
            rcs_thrusters.append(RCSThruster(
                thruster_id=f"{random.choice(positions)}{i+1}",
                status=random.choice(["Active"] * 9 + ["Disabled"]),
                firing_count=random.randint(100, 1000),
                propellant_used=random.uniform(1, 5)
            ))
        
        fuel = Fuel(
            hydrazine=random.uniform(200, 300),
            mon_oxidizer=random.uniform(180, 280),
            total_delta_v=random.uniform(1200, 1500),
            used_delta_v=random.uniform(200, 400)
        )
        
        upcoming_burns = [
            UpcomingBurn(
                name="Mid-course Correction",
                time=self.current_time + timedelta(hours=48),
                duration=30.0,
                delta_v=15.0
            ),
            UpcomingBurn(
                name="TEI (Trans-Earth Injection)",
                time=self.current_time + timedelta(days=8),
                duration=180.0,
                delta_v=450.0
            )
        ]
        
        return PropulsionSystem(
            main_engine=main_engine,
            rcs_thrusters=rcs_thrusters,
            fuel=fuel,
            upcoming_burns=upcoming_burns,
            timestamp=self.current_time
        )
    
    def generate_communications(self) -> Communications:
        """Generate communications data"""
        dsn_contacts = [
            DSNContact(
                station="Goldstone",
                status="Active",
                signal_strength=random.uniform(-110, -90),
                data_rate=random.uniform(2.0, 5.0),
                uplink_freq=2106.4,
                downlink_freq=2287.5,
                acquisition_time=self.current_time - timedelta(minutes=30),
                loss_of_signal=self.current_time + timedelta(minutes=45)
            )
        ]
        
        current_downlink = CurrentDownlink(
            video=random.random() > 0.7,
            telemetry=random.uniform(50, 100),
            voice=random.random() > 0.3,
            delay=random.uniform(1.2, 1.4)
        )
        
        return Communications(
            dsn_contacts=dsn_contacts,
            current_downlink=current_downlink,
            message_queue=random.randint(0, 5),
            timestamp=self.current_time
        )
    
    def generate_mission_timeline(self) -> MissionTimeline:
        """Generate mission timeline"""
        events = [
            MissionEvent(
                met="T+0:00:00",
                event="Liftoff",
                status="Complete",
                description="SLS liftoff from Kennedy Space Center LC-39B"
            ),
            MissionEvent(
                met="T+0:01:40",
                event="Trans-Lunar Injection (TLI)",
                status="Complete",
                description="ICPS upper stage burn to lunar trajectory"
            ),
            MissionEvent(
                met="T+8:12:00",
                event="Lunar Flyby",
                status="Upcoming",
                description="Closest approach to Moon (~130 km altitude)"
            ),
            MissionEvent(
                met="T+10:00:00",
                event="Splashdown",
                status="Upcoming",
                description="Pacific Ocean landing"
            )
        ]
        
        return MissionTimeline(
            events=events,
            current_phase="Trans-Lunar Coast",
            next_major_event=events[2]
        )
    
    def generate_all_telemetry(self) -> Dict[str, Any]:
        """Generate complete telemetry snapshot"""
        return {
            "spacecraft": self.generate_spacecraft_telemetry().dict(),
            "crew": [self.generate_crew_member(i).dict() for i in range(4)],
            "eclss": self.generate_eclss().dict(),
            "power": self.generate_power_system().dict(),
            "propulsion": self.generate_propulsion_system().dict(),
            "communications": self.generate_communications().dict(),
            "timeline": self.generate_mission_timeline().dict()
        }
