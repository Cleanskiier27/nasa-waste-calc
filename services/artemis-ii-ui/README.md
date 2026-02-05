# Artemis II Mission Control UI

## Overview

React-based frontend application for the Artemis II Mission Control System. Features real-time 3D spacecraft visualization, telemetry displays, crew health monitoring, and systems status.

## Features

- **3D Spacecraft Visualization**: Three.js-based Earth-Moon-spacecraft rendering
- **Real-time Telemetry**: WebSocket connection for live updates (10 Hz)
- **Crew Health Dashboard**: Vital signs for all 4 astronauts
- **Systems Monitoring**: ECLSS, Power, Propulsion, Communications
- **Mission Timeline**: Event tracking with visual indicators
- **Alert System**: Real-time alerts with acknowledgment

## Installation

```bash
# Install dependencies
npm install

# Start development server
npm start

# Build for production
npm run build
```

## Configuration

Create a `.env` file:

```
REACT_APP_API_URL=http://localhost:8081
```

## Docker

```bash
# Build image
docker build -t artemis-ii-ui .

# Run container
docker run -p 3001:80 artemis-ii-ui
```

## Project Structure

```
src/
├── components/
│   ├── MissionControl.jsx    # Main dashboard
│   ├── SpacecraftView.jsx     # 3D visualization
│   ├── TelemetryPanel.jsx     # Telemetry display
│   ├── CrewHealth.jsx         # Crew monitoring
│   ├── SystemStatus.jsx       # Systems status
│   ├── Timeline.jsx           # Mission timeline
│   └── AlertPanel.jsx         # Alerts
├── hooks/
│   └── useTelemetry.js        # WebSocket hook
├── styles/
│   ├── index.css
│   └── mission-control.css    # Dashboard styles
├── App.jsx
└── index.js
```

## Usage

1. Start the backend API server (port 8081)
2. Start the frontend application:
   ```bash
   npm start
   ```
3. Open http://localhost:3000

The dashboard will automatically connect to the backend WebSocket and begin streaming telemetry data.

## Components

### MissionControl
Main dashboard container with layout and WebSocket management.

### SpacecraftView
3D visualization using Three.js and React Three Fiber:
- Earth and Moon rendering
- Spacecraft position indicator
- Trajectory path
- Orbit controls (zoom, pan, rotate)

### TelemetryPanel
Displays spacecraft telemetry:
- Position and velocity vectors
- Altitude and distances
- Orbital parameters

### CrewHealth
Monitors vital signs for 4 astronauts:
- Heart rate
- Blood pressure
- Oxygen saturation
- Temperature
- Radiation exposure
- Sleep hours
- Exercise compliance

### SystemStatus
Shows status of all spacecraft systems:
- ECLSS (Environmental Control)
- Power System
- Propulsion
- Communications

### Timeline
Mission event tracking:
- Current phase indicator
- Event list with status
- Next major event countdown

### AlertPanel
Alert management:
- Active alerts by severity
- Acknowledgment system
- Alert history

## Technologies

- React 18
- Three.js / React Three Fiber
- Chart.js
- WebSocket
- Axios

## License

Part of the NASA NetworkBuster Initiative
