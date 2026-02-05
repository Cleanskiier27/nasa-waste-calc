"""
Orbital mechanics calculations for Artemis II trajectory
"""
import math
import numpy as np
from typing import Tuple
from datetime import datetime, timedelta


# Physical constants
EARTH_RADIUS = 6371.0  # km
MOON_RADIUS = 1737.4  # km
EARTH_MU = 398600.4418  # km^3/s^2 (gravitational parameter)
MOON_MU = 4902.8  # km^3/s^2
EARTH_MOON_DISTANCE = 384400.0  # km (average)


class OrbitalMechanics:
    """Orbital mechanics calculations"""
    
    def __init__(self, mission_start_time: datetime):
        self.mission_start_time = mission_start_time
        self.current_time = mission_start_time
        
    def set_time(self, current_time: datetime):
        """Update current mission time"""
        self.current_time = current_time
        
    def get_mission_elapsed_time(self) -> str:
        """Calculate mission elapsed time in format DD:HH:MM:SS"""
        elapsed = self.current_time - self.mission_start_time
        days = elapsed.days
        hours = elapsed.seconds // 3600
        minutes = (elapsed.seconds % 3600) // 60
        seconds = elapsed.seconds % 60
        return f"{days:02d}:{hours:02d}:{minutes:02d}:{seconds:02d}"
    
    def calculate_orbital_velocity(self, r: float, a: float) -> float:
        """
        Calculate orbital velocity using vis-viva equation
        r: current distance from center of body (km)
        a: semi-major axis (km)
        """
        return math.sqrt(EARTH_MU * (2/r - 1/a))
    
    def calculate_orbital_parameters(self, position: np.ndarray, velocity: np.ndarray) -> dict:
        """
        Calculate orbital parameters from position and velocity vectors
        """
        # Distance from Earth
        r = np.linalg.norm(position)
        v = np.linalg.norm(velocity)
        
        # Specific orbital energy
        energy = (v**2 / 2) - (EARTH_MU / r)
        
        # Semi-major axis
        a = -EARTH_MU / (2 * energy)
        
        # Angular momentum vector
        h_vec = np.cross(position, velocity)
        h = np.linalg.norm(h_vec)
        
        # Eccentricity vector
        e_vec = ((v**2 - EARTH_MU/r) * position - np.dot(position, velocity) * velocity) / EARTH_MU
        e = np.linalg.norm(e_vec)
        
        # Inclination
        inclination = math.degrees(math.acos(h_vec[2] / h))
        
        # Apogee and perigee
        apogee = a * (1 + e) - EARTH_RADIUS
        perigee = a * (1 - e) - EARTH_RADIUS
        
        # Orbital period
        period = 2 * math.pi * math.sqrt(a**3 / EARTH_MU)
        
        return {
            "apogee": max(apogee, 0),
            "perigee": max(perigee, 0),
            "inclination": inclination,
            "period": period
        }
    
    def propagate_position(self, t_seconds: float) -> Tuple[np.ndarray, np.ndarray]:
        """
        Propagate spacecraft position for Artemis II trajectory
        Simplified trans-lunar trajectory model
        t_seconds: mission elapsed time in seconds
        """
        # Mission phases
        t_hours = t_seconds / 3600.0
        
        if t_hours < 2:
            # Launch and LEO parking orbit (first 2 hours)
            angle = (t_seconds / 5400.0) * 2 * math.pi  # 90-minute orbit
            altitude = 185 + (t_hours / 2.0) * 15  # Gradually increasing
            r = EARTH_RADIUS + altitude
            position = np.array([
                r * math.cos(angle),
                r * math.sin(angle),
                r * 0.1  # Slight inclination
            ])
            v = 7.8 - (t_hours / 2.0) * 0.2  # Orbital velocity
            velocity = np.array([
                -v * math.sin(angle),
                v * math.cos(angle),
                0.1
            ])
            
        elif t_hours < 100:
            # Trans-lunar injection to lunar flyby (2-100 hours)
            progress = (t_hours - 2) / 98.0
            
            # Trajectory from Earth to Moon
            earth_distance = EARTH_RADIUS + 200 + progress * (EARTH_MOON_DISTANCE - EARTH_RADIUS - 200)
            angle = progress * 0.3  # Slight curve
            
            position = np.array([
                earth_distance * math.cos(angle),
                earth_distance * math.sin(angle),
                earth_distance * 0.05
            ])
            
            # Velocity decreases as we move away from Earth
            v = 10.9 * math.exp(-progress * 2.5) + 1.0
            velocity = np.array([
                -v * math.sin(angle),
                v * math.cos(angle),
                0.05
            ])
            
        else:
            # Return trajectory (after 100 hours)
            progress = (t_hours - 100) / 140.0  # Return over 140 hours
            progress = min(progress, 1.0)
            
            # Coming back to Earth
            earth_distance = EARTH_MOON_DISTANCE - progress * (EARTH_MOON_DISTANCE - EARTH_RADIUS - 200)
            angle = 0.3 + progress * 0.5
            
            position = np.array([
                earth_distance * math.cos(angle),
                earth_distance * math.sin(angle),
                earth_distance * 0.03
            ])
            
            # Velocity increases as we approach Earth
            v = 1.5 + progress * 10.0
            velocity = np.array([
                -v * math.sin(angle),
                v * math.cos(angle),
                -0.1 * progress
            ])
        
        return position, velocity
    
    def calculate_distances(self, position: np.ndarray) -> Tuple[float, float, float]:
        """
        Calculate distances from Earth and Moon
        Returns: (altitude, distance_earth, distance_moon)
        """
        distance_earth = np.linalg.norm(position)
        altitude = distance_earth - EARTH_RADIUS
        
        # Simplified Moon position (assume it's at a fixed location for simulation)
        moon_position = np.array([EARTH_MOON_DISTANCE, 0, 0])
        distance_moon = np.linalg.norm(position - moon_position)
        
        return altitude, distance_earth, distance_moon
