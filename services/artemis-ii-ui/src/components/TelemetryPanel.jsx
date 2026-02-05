import React from 'react';

function formatNumber(num, decimals = 2) {
  if (num === null || num === undefined) return 'N/A';
  return Number(num).toFixed(decimals);
}

function TelemetryPanel({ telemetryData }) {
  if (!telemetryData) {
    return (
      <div className="telemetry-panel">
        <div className="panel-header">
          <h3>📡 SPACECRAFT TELEMETRY</h3>
        </div>
        <div className="panel-content">
          <p>Connecting to spacecraft...</p>
        </div>
      </div>
    );
  }

  const { spacecraft } = telemetryData;

  return (
    <div className="telemetry-panel panel">
      <div className="panel-header">
        <h3>📡 SPACECRAFT TELEMETRY</h3>
        <span className="status-indicator status-nominal">NOMINAL</span>
      </div>
      <div className="panel-content">
        <div className="telemetry-grid">
          <div className="telemetry-item">
            <span className="label">MISSION ELAPSED TIME</span>
            <span className="value large">{spacecraft.mission_elapsed_time}</span>
          </div>
          
          <div className="telemetry-section">
            <h4>Position (km)</h4>
            <div className="telemetry-row">
              <div className="telemetry-item">
                <span className="label">X</span>
                <span className="value">{formatNumber(spacecraft.position.x, 1)}</span>
              </div>
              <div className="telemetry-item">
                <span className="label">Y</span>
                <span className="value">{formatNumber(spacecraft.position.y, 1)}</span>
              </div>
              <div className="telemetry-item">
                <span className="label">Z</span>
                <span className="value">{formatNumber(spacecraft.position.z, 1)}</span>
              </div>
            </div>
          </div>

          <div className="telemetry-section">
            <h4>Velocity (km/s)</h4>
            <div className="telemetry-row">
              <div className="telemetry-item">
                <span className="label">X</span>
                <span className="value">{formatNumber(spacecraft.velocity.x, 3)}</span>
              </div>
              <div className="telemetry-item">
                <span className="label">Y</span>
                <span className="value">{formatNumber(spacecraft.velocity.y, 3)}</span>
              </div>
              <div className="telemetry-item">
                <span className="label">Z</span>
                <span className="value">{formatNumber(spacecraft.velocity.z, 3)}</span>
              </div>
            </div>
          </div>

          <div className="telemetry-item">
            <span className="label">ALTITUDE</span>
            <span className="value">{formatNumber(spacecraft.altitude, 0)} km</span>
          </div>

          <div className="telemetry-item">
            <span className="label">DISTANCE FROM EARTH</span>
            <span className="value">{formatNumber(spacecraft.distance_earth, 0)} km</span>
          </div>

          <div className="telemetry-item">
            <span className="label">DISTANCE FROM MOON</span>
            <span className="value">{formatNumber(spacecraft.distance_moon, 0)} km</span>
          </div>

          <div className="telemetry-section">
            <h4>Orbital Parameters</h4>
            <div className="telemetry-item">
              <span className="label">APOGEE</span>
              <span className="value">{formatNumber(spacecraft.orbital_parameters.apogee, 0)} km</span>
            </div>
            <div className="telemetry-item">
              <span className="label">PERIGEE</span>
              <span className="value">{formatNumber(spacecraft.orbital_parameters.perigee, 0)} km</span>
            </div>
            <div className="telemetry-item">
              <span className="label">INCLINATION</span>
              <span className="value">{formatNumber(spacecraft.orbital_parameters.inclination, 2)}°</span>
            </div>
            <div className="telemetry-item">
              <span className="label">PERIOD</span>
              <span className="value">{formatNumber(spacecraft.orbital_parameters.period / 60, 0)} min</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}

export default TelemetryPanel;
