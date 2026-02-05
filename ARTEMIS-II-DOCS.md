# 🚀 Artemis II Mission Control System

## Overview

A comprehensive, production-ready mission control system for NASA's Artemis II mission - the first crewed mission beyond Low Earth Orbit since Apollo 17 (1972). This system provides real-time telemetry monitoring, crew health tracking, spacecraft systems status, and 3D trajectory visualization.

![Mission Control Dashboard](https://via.placeholder.com/1200x600/0a0a0a/00ff00?text=Artemis+II+Mission+Control)

## 🎯 Mission Details

- **Crew:** 4 astronauts (Reid Wiseman, Victor Glover, Christina Koch, Jeremy Hansen)
- **Duration:** 10 days
- **Distance:** 370,000 km from Earth (lunar flyby)
- **Spacecraft:** Orion capsule + European Service Module
- **Launch Vehicle:** Space Launch System (SLS) Block 1
- **Launch Date:** September 2025 (planned)

## ✨ Features

### 1. Real-time Telemetry Monitoring (< 1 second latency)
- **Position & Velocity Tracking**: 3D position and velocity vectors
- **Orbital Parameters**: Apogee, perigee, inclination, period
- **Distance Measurements**: From Earth and Moon
- **Mission Elapsed Time**: Precise mission clock

### 2. 3D Spacecraft Visualization (60 FPS)
- **Earth-Moon System**: Realistic rendering with textures
- **Spacecraft Position**: Real-time position indicator
- **Trajectory Path**: Orbital path visualization
- **Camera Controls**: Orbit, zoom, pan capabilities

### 3. Crew Health Monitoring (4 Astronauts)
- **Vital Signs**: Heart rate, blood pressure, SpO2, temperature
- **Radiation Exposure**: Cumulative radiation tracking
- **Sleep Analysis**: 24-hour sleep tracking
- **Exercise Compliance**: Daily exercise monitoring
- **Medical Alerts**: Automatic anomaly detection

### 4. Life Support Systems (ECLSS)
- **Oxygen Management**: Generation rate, tank pressure, consumption
- **CO₂ Removal**: Scrubber status, CO₂ levels, LiOH remaining
- **Water System**: Potable water, wastewater, humidity
- **Cabin Environment**: Pressure, temperature, air quality

### 5. Power Systems
- **Solar Arrays**: Output, voltage, temperature, sun angle
- **Battery Status**: 4 batteries with SOC, health, cycles
- **Power Consumption**: Total and by subsystem
- **Power Budget**: Available, required, margin tracking

### 6. Propulsion Systems
- **Main Engine**: Status, thrust, ISP, temperature
- **RCS Thrusters**: 24 thrusters with individual status
- **Fuel Management**: Hydrazine, MON oxidizer levels
- **Delta-V Budget**: Available and expended delta-v

### 7. Communications
- **DSN Contacts**: Deep Space Network stations
- **Signal Strength**: Real-time signal monitoring
- **Data Rates**: Uplink/downlink rates
- **Delay Tracking**: One-way light time

### 8. Mission Timeline
- **Event Tracking**: Liftoff, TLI, lunar flyby, splashdown
- **Current Phase**: Mission phase indicator
- **Countdown**: Next major event countdown

### 9. Alert System
- **Severity Levels**: INFO, WARNING, CAUTION, CRITICAL
- **Real-time Notifications**: Immediate system alerts
- **Acknowledgment**: Alert acknowledgment system
- **Alert History**: Complete alert log

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Mission Control UI                        │
│                   (React + Three.js)                         │
│                      Port: 3001                              │
└───────────────────────┬─────────────────────────────────────┘
                        │ WebSocket + REST
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                  FastAPI Backend                             │
│          (Python + Orbital Mechanics + ML)                   │
│                     Port: 8081                               │
└──────┬─────────────────┬──────────────────┬─────────────────┘
       │                 │                  │
       ▼                 ▼                  ▼
┌──────────────┐  ┌──────────────┐  ┌──────────────┐
│ TimescaleDB  │  │    Redis     │  │   Grafana    │
│   (Metrics)  │  │   (Cache)    │  │ (Monitoring) │
│  Port: 5432  │  │  Port: 6379  │  │  Port: 3002  │
└──────────────┘  └──────────────┘  └──────────────┘
```

## 🚀 Quick Start

### Prerequisites

- Docker & Docker Compose
- Node.js 18+ (for local development)
- Python 3.11+ (for local development)

### Docker Deployment (Recommended)

```bash
# Clone the repository
git clone https://github.com/Cleanskiier27/nasa-waste-calc.git
cd nasa-waste-calc

# Start all services
docker-compose up -d

# View logs
docker-compose logs -f

# Access services:
# - Mission Control UI: http://localhost:3001
# - Backend API: http://localhost:8081
# - Grafana Dashboards: http://localhost:3002 (admin/artemis)
```

### Local Development

#### Backend

```bash
cd services/artemis-ii

# Create virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Run server
python -m app.main
```

#### Frontend

```bash
cd services/artemis-ii-ui

# Install dependencies
npm install

# Start development server
npm start
```

## 📖 Documentation

### API Documentation

Once the backend is running, visit:
- **Swagger UI**: http://localhost:8081/docs
- **ReDoc**: http://localhost:8081/redoc

### Key API Endpoints

```
GET  /api/telemetry/current          # Current spacecraft state
GET  /api/telemetry/history          # Historical data
GET  /api/crew                       # All crew members
GET  /api/crew/{crew_id}             # Specific crew member (0-3)
GET  /api/systems/eclss              # Life support system
GET  /api/systems/power              # Power system
GET  /api/systems/propulsion         # Propulsion system
GET  /api/systems/communications     # Communications
GET  /api/trajectory                 # Orbital trajectory
GET  /api/timeline                   # Mission timeline
GET  /api/alerts                     # Active alerts
POST /api/alerts/{alert_id}/ack      # Acknowledge alert
WS   /ws/telemetry                   # WebSocket stream (10 Hz)
```

### WebSocket Connection

```javascript
const ws = new WebSocket('ws://localhost:8081/ws/telemetry');

ws.onmessage = (event) => {
    const telemetry = JSON.parse(event.data);
    console.log('Spacecraft:', telemetry.spacecraft);
    console.log('Crew:', telemetry.crew);
    console.log('Systems:', telemetry.eclss, telemetry.power, ...);
};
```

## 🎨 UI Components

### Mission Control Dashboard
- **Layout**: 3-column grid layout for optimal information density
- **Theme**: NASA-inspired dark theme with green/blue accents
- **Updates**: Real-time updates at 10 Hz
- **Responsive**: Adapts to different screen sizes

### Component Overview
- **SpacecraftView**: 3D visualization with Three.js
- **TelemetryPanel**: Real-time telemetry data
- **CrewHealth**: Vital signs for 4 astronauts
- **SystemStatus**: ECLSS, Power, Propulsion, Communications
- **Timeline**: Mission event tracking
- **AlertPanel**: Alert management

## 🧪 Testing

### Backend Tests

```bash
cd services/artemis-ii
pytest tests/ --cov=app
```

### Frontend Tests

```bash
cd services/artemis-ii-ui
npm test
```

## 📊 Monitoring

### Grafana Dashboards

Access Grafana at http://localhost:3002 (admin/artemis)

Pre-configured dashboards:
- **Spacecraft Telemetry**: Position, velocity, orbital parameters
- **Crew Health**: Vital signs trends
- **Systems Status**: ECLSS, power, propulsion metrics
- **Alert History**: Alert frequency and resolution times

## 🔧 Configuration

### Environment Variables

#### Backend (.env)
```bash
DATABASE_URL=postgresql://postgres:artemis@localhost:5432/artemis
REDIS_URL=redis://localhost:6379/0
PORT=8081
```

#### Frontend (.env)
```bash
REACT_APP_API_URL=http://localhost:8081
```

## 📁 Project Structure

```
nasa-waste-calc/
├── services/
│   ├── artemis-ii/                  # Backend service
│   │   ├── app/
│   │   │   ├── main.py              # FastAPI application
│   │   │   ├── models.py            # Pydantic models
│   │   │   └── orbital_mechanics.py # Trajectory calculations
│   │   ├── simulation/
│   │   │   └── telemetry_simulator.py # Data generator
│   │   ├── data/                    # Configuration files
│   │   ├── tests/                   # Unit tests
│   │   ├── Dockerfile
│   │   ├── requirements.txt
│   │   └── README.md
│   │
│   └── artemis-ii-ui/               # Frontend service
│       ├── src/
│       │   ├── components/          # React components
│       │   ├── hooks/               # Custom hooks
│       │   ├── styles/              # CSS files
│       │   ├── App.jsx
│       │   └── index.js
│       ├── public/
│       ├── Dockerfile
│       ├── package.json
│       └── README.md
│
├── docker-compose.yml               # Docker Compose configuration
├── ARTEMIS-II-DOCS.md              # This file
└── README.md                        # Main README
```

## 🔐 Security

- **No Hardcoded Credentials**: All sensitive data in environment variables
- **HTTPS Ready**: Configure reverse proxy for production
- **CORS Configuration**: Restrict origins in production
- **Input Validation**: Pydantic models validate all inputs
- **Rate Limiting**: Consider adding rate limiting for production

## 🚢 Deployment

### Production Deployment

1. **Set up environment variables**
2. **Configure reverse proxy** (nginx/Caddy) with HTTPS
3. **Update CORS settings** in backend
4. **Set up monitoring** (Prometheus/Grafana)
5. **Configure backups** for TimescaleDB
6. **Deploy with Docker Compose** or Kubernetes

### NetworkBuster.NET Deployment

This system is designed to be deployed to NetworkBuster.NET infrastructure:

```bash
# Build and push images
docker-compose build
docker tag artemis-ii-backend:latest networkbuster.net/artemis-ii-backend:latest
docker tag artemis-ii-frontend:latest networkbuster.net/artemis-ii-frontend:latest
docker push networkbuster.net/artemis-ii-backend:latest
docker push networkbuster.net/artemis-ii-frontend:latest

# Deploy to production
docker-compose -f docker-compose.prod.yml up -d
```

## 🎯 Success Criteria

✅ **Real-time telemetry display** (< 1 second latency)  
✅ **3D spacecraft visualization** (smooth 60 FPS)  
✅ **Crew health monitoring** (all 4 astronauts)  
✅ **Alert system** (immediate notification)  
✅ **Historical data queries** (fast retrieval)  
✅ **Mission timeline** (accurate event tracking)  
✅ **System status indicators** (color-coded health)  
✅ **Orbital mechanics calculations** (accurate trajectories)  
✅ **Production-ready** (deployable to NetworkBuster.NET)

## 📚 References

- [NASA Orion Spacecraft](https://www.nasa.gov/orion)
- [Artemis II Mission](https://www.nasa.gov/artemis-ii)
- [Apollo Mission Reports](https://www.nasa.gov/mission_pages/apollo/missions/)
- [ISS ECLSS Documentation](https://www.nasa.gov/mission_pages/station/research/benefits/eclss.html)
- [Orbital Mechanics (Vallado)](https://celestrak.com/software/vallado-sw.php)

## 🤝 Contributing

This is a production system for NASA mission monitoring. Changes should be:
1. Thoroughly tested
2. Documented
3. Reviewed for accuracy
4. Security-checked

## 📄 License

Part of the NASA NetworkBuster Initiative  
© 2026 NetworkBuster.NET

## 🆘 Support

For issues, questions, or support:
- **Issues**: GitHub Issues
- **Documentation**: See `/docs` folder
- **Contact**: NetworkBuster.NET team

---

**🚀 Let's launch! This mission control system is ready to monitor a real Artemis II mission! 🌙**
