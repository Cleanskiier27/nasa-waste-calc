# Artemis II Mission Control Backend

## Overview

FastAPI-based backend service for the Artemis II Mission Control System. Provides real-time telemetry, crew health monitoring, systems status, and WebSocket streaming.

## Features

- **Real-time Telemetry**: 10 Hz WebSocket updates
- **Spacecraft Tracking**: Position, velocity, orbital parameters
- **Crew Health**: Vital signs for 4 astronauts
- **Systems Monitoring**: ECLSS, Power, Propulsion, Communications
- **Mission Timeline**: Event tracking and timeline management
- **Alert System**: Real-time alerts with acknowledgment

## API Endpoints

### Telemetry
- `GET /api/telemetry/current` - Current spacecraft state
- `GET /api/telemetry/history` - Historical data
- `WS /ws/telemetry` - WebSocket stream (10 Hz)

### Crew
- `GET /api/crew` - All crew members
- `GET /api/crew/{crew_id}` - Specific crew member (0-3)

### Systems
- `GET /api/systems/eclss` - Life support system
- `GET /api/systems/power` - Power system
- `GET /api/systems/propulsion` - Propulsion system
- `GET /api/systems/communications` - Communications

### Mission
- `GET /api/trajectory` - Orbital trajectory
- `GET /api/timeline` - Mission timeline
- `GET /api/alerts` - Active alerts
- `POST /api/alerts/{alert_id}/ack` - Acknowledge alert

### Utility
- `GET /` - Service status
- `GET /api/health` - Health check

## Installation

```bash
# Install dependencies
pip install -r requirements.txt

# Run the server
python -m app.main

# Or use uvicorn directly
uvicorn app.main:app --host 0.0.0.0 --port 8081 --reload
```

## Docker

```bash
# Build image
docker build -t artemis-ii-backend .

# Run container
docker run -p 8081:8081 artemis-ii-backend
```

## Development

### Project Structure
```
services/artemis-ii/
├── app/
│   ├── main.py              # FastAPI application
│   ├── models.py            # Pydantic models
│   └── orbital_mechanics.py # Trajectory calculations
├── simulation/
│   └── telemetry_simulator.py # Data generator
├── data/                    # Configuration files
├── tests/                   # Unit tests
├── Dockerfile
├── requirements.txt
└── README.md
```

### Testing

```bash
# Run tests
pytest tests/

# With coverage
pytest --cov=app tests/
```

## WebSocket Usage

Connect to `ws://localhost:8081/ws/telemetry` for real-time updates.

Example (JavaScript):
```javascript
const ws = new WebSocket('ws://localhost:8081/ws/telemetry');

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    console.log('Telemetry:', data);
};
```

## Configuration

Environment variables:
- `DATABASE_URL` - PostgreSQL/TimescaleDB connection
- `REDIS_URL` - Redis connection for caching
- `PORT` - Server port (default: 8081)

## License

Part of the NASA NetworkBuster Initiative
