import React from 'react';
import { useTelemetry } from '../hooks/useTelemetry';
import SpacecraftView from './SpacecraftView';
import TelemetryPanel from './TelemetryPanel';
import CrewHealth from './CrewHealth';
import SystemStatus from './SystemStatus';
import Timeline from './Timeline';
import AlertPanel from './AlertPanel';

function MissionControl() {
  const { telemetryData, connected, error } = useTelemetry();

  return (
    <div className="mission-control">
      {/* Header */}
      <header className="mission-header">
        <div className="header-content">
          <div className="mission-title">
            <h1>🚀 ARTEMIS II MISSION CONTROL</h1>
            <p>NASA's First Crewed Mission Beyond Low Earth Orbit Since Apollo 17</p>
          </div>
          <div className="connection-status">
            <div className={`status-dot ${connected ? 'connected' : 'disconnected'}`}></div>
            <span>{connected ? 'CONNECTED' : 'DISCONNECTED'}</span>
            {telemetryData && (
              <div className="met-display">
                MET: {telemetryData.spacecraft?.mission_elapsed_time || 'N/A'}
              </div>
            )}
          </div>
        </div>
      </header>

      {/* Error Display */}
      {error && (
        <div className="error-banner">
          ⚠️ {error}
        </div>
      )}

      {/* Main Dashboard */}
      <div className="dashboard-container">
        {/* Left Column - 3D View and Telemetry */}
        <div className="dashboard-column left-column">
          <div className="spacecraft-container">
            <div className="panel-header">
              <h3>🌍 3D TRAJECTORY VIEW</h3>
            </div>
            <SpacecraftView telemetryData={telemetryData} />
          </div>
          <TelemetryPanel telemetryData={telemetryData} />
        </div>

        {/* Middle Column - Crew and Systems */}
        <div className="dashboard-column middle-column">
          <CrewHealth telemetryData={telemetryData} />
          <SystemStatus telemetryData={telemetryData} />
        </div>

        {/* Right Column - Timeline and Alerts */}
        <div className="dashboard-column right-column">
          <Timeline telemetryData={telemetryData} />
          <AlertPanel telemetryData={telemetryData} />
        </div>
      </div>

      {/* Footer */}
      <footer className="mission-footer">
        <div className="footer-content">
          <span>NetworkBuster.NET | NASA Artemis II Mission Control System</span>
          <span>Real-time Telemetry Stream | 10 Hz Update Rate</span>
          <span className="timestamp">
            {new Date().toISOString().replace('T', ' ').substring(0, 19)} UTC
          </span>
        </div>
      </footer>
    </div>
  );
}

export default MissionControl;
