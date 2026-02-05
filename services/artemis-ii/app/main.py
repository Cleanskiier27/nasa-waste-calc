"""
FastAPI application for Artemis II Mission Control System
"""
from fastapi import FastAPI, WebSocket, WebSocketDisconnect, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import JSONResponse
from datetime import datetime, timedelta
from typing import List, Dict, Any
import asyncio
import json
import sys
import os

# Add parent directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from app.models import *
from simulation.telemetry_simulator import TelemetrySimulator


# Initialize FastAPI app
app = FastAPI(
    title="Artemis II Mission Control API",
    description="Real-time telemetry and mission control for NASA Artemis II",
    version="1.0.0"
)

# CORS middleware
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # In production, specify exact origins
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Global telemetry simulator
simulator = TelemetrySimulator(mission_start_time=datetime.utcnow())

# Active WebSocket connections
active_connections: List[WebSocket] = []

# Alert storage
alerts: List[Alert] = []


class ConnectionManager:
    """Manage WebSocket connections"""
    
    def __init__(self):
        self.active_connections: List[WebSocket] = []
    
    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)
    
    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)
    
    async def broadcast(self, message: dict):
        """Broadcast message to all connected clients"""
        for connection in self.active_connections:
            try:
                await connection.send_json(message)
            except Exception:
                pass


manager = ConnectionManager()


@app.get("/")
async def root():
    """API root"""
    return {
        "service": "Artemis II Mission Control",
        "status": "operational",
        "mission_elapsed_time": simulator.orbital_mechanics.get_mission_elapsed_time(),
        "timestamp": datetime.utcnow().isoformat()
    }


@app.get("/api/telemetry/current")
async def get_current_telemetry():
    """Get current spacecraft telemetry"""
    try:
        telemetry = simulator.generate_spacecraft_telemetry()
        return telemetry
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/telemetry/history")
async def get_telemetry_history(
    start_time: str = None,
    end_time: str = None,
    limit: int = 100
):
    """Get historical telemetry data"""
    # In a real system, this would query TimescaleDB
    # For simulation, return current data
    return {
        "message": "Historical data endpoint - would query TimescaleDB in production",
        "current_data": simulator.generate_spacecraft_telemetry()
    }


@app.get("/api/crew")
async def get_all_crew():
    """Get all crew members' health data"""
    try:
        crew = [simulator.generate_crew_member(i) for i in range(4)]
        return crew
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/crew/{crew_id}")
async def get_crew_member(crew_id: int):
    """Get specific crew member health data"""
    if crew_id < 0 or crew_id > 3:
        raise HTTPException(status_code=404, detail="Crew member not found")
    
    try:
        crew_member = simulator.generate_crew_member(crew_id)
        return crew_member
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/trajectory")
async def get_trajectory():
    """Get orbital trajectory data"""
    try:
        telemetry = simulator.generate_spacecraft_telemetry()
        return {
            "position": telemetry.position,
            "velocity": telemetry.velocity,
            "orbital_parameters": telemetry.orbital_parameters,
            "mission_elapsed_time": telemetry.mission_elapsed_time
        }
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/systems/eclss")
async def get_eclss():
    """Get ECLSS (Environmental Control and Life Support System) status"""
    try:
        eclss = simulator.generate_eclss()
        return eclss
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/systems/power")
async def get_power_system():
    """Get power system status"""
    try:
        power = simulator.generate_power_system()
        return power
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/systems/propulsion")
async def get_propulsion_system():
    """Get propulsion system status"""
    try:
        propulsion = simulator.generate_propulsion_system()
        return propulsion
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/systems/communications")
async def get_communications():
    """Get communications system status"""
    try:
        comms = simulator.generate_communications()
        return comms
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/timeline")
async def get_mission_timeline():
    """Get mission timeline"""
    try:
        timeline = simulator.generate_mission_timeline()
        return timeline
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/api/alerts")
async def get_alerts(active_only: bool = True):
    """Get alerts"""
    if active_only:
        return [alert for alert in alerts if not alert.acknowledged]
    return alerts


@app.post("/api/alerts/{alert_id}/ack")
async def acknowledge_alert(alert_id: int):
    """Acknowledge an alert"""
    for alert in alerts:
        if id(alert) == alert_id:
            alert.acknowledged = True
            return {"status": "acknowledged", "alert_id": alert_id}
    
    raise HTTPException(status_code=404, detail="Alert not found")


@app.websocket("/ws/telemetry")
async def websocket_telemetry(websocket: WebSocket):
    """WebSocket endpoint for real-time telemetry updates"""
    await manager.connect(websocket)
    
    try:
        # Send initial data
        telemetry = simulator.generate_all_telemetry()
        await websocket.send_json(telemetry)
        
        # Keep connection alive and send periodic updates
        while True:
            await asyncio.sleep(0.1)  # 10 Hz update rate
            
            # Advance simulation
            simulator.advance_time(0.1)
            
            # Generate new telemetry
            telemetry = simulator.generate_all_telemetry()
            
            # Send to this client
            await websocket.send_json(telemetry)
            
    except WebSocketDisconnect:
        manager.disconnect(websocket)
    except Exception as e:
        print(f"WebSocket error: {e}")
        manager.disconnect(websocket)


@app.get("/api/health")
async def health_check():
    """Health check endpoint"""
    return {
        "status": "healthy",
        "timestamp": datetime.utcnow().isoformat(),
        "mission_elapsed_time": simulator.orbital_mechanics.get_mission_elapsed_time(),
        "active_websocket_connections": len(manager.active_connections)
    }


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8081, log_level="info")
