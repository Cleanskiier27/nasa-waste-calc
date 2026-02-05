import React from 'react';

function getVitalStatus(vital, min, max) {
  if (vital < min || vital > max) return 'critical';
  if (vital < min * 1.1 || vital > max * 0.9) return 'warning';
  return 'nominal';
}

function CrewHealth({ telemetryData }) {
  if (!telemetryData || !telemetryData.crew) {
    return (
      <div className="crew-health panel">
        <div className="panel-header">
          <h3>👨‍🚀 CREW HEALTH</h3>
        </div>
        <div className="panel-content">
          <p>Loading crew data...</p>
        </div>
      </div>
    );
  }

  const { crew } = telemetryData;

  return (
    <div className="crew-health panel">
      <div className="panel-header">
        <h3>👨‍🚀 CREW HEALTH MONITORING</h3>
        <span className="status-indicator status-nominal">ALL NOMINAL</span>
      </div>
      <div className="panel-content">
        <div className="crew-grid">
          {crew.map((member, index) => {
            const hrStatus = getVitalStatus(member.vitals.heart_rate, 60, 100);
            const o2Status = getVitalStatus(member.vitals.oxygen_saturation, 95, 100);
            const tempStatus = getVitalStatus(member.vitals.temperature, 36.5, 37.5);

            return (
              <div key={index} className="crew-card">
                <div className="crew-header">
                  <h4>{member.name}</h4>
                  <span className="crew-role">{member.role}</span>
                </div>
                <div className="crew-vitals">
                  <div className={`vital-item status-${hrStatus}`}>
                    <span className="vital-label">❤️ HEART RATE</span>
                    <span className="vital-value">{member.vitals.heart_rate} BPM</span>
                  </div>
                  <div className="vital-item status-${o2Status}">
                    <span className="vital-label">💨 SpO2</span>
                    <span className="vital-value">{member.vitals.oxygen_saturation}%</span>
                  </div>
                  <div className={`vital-item status-${tempStatus}`}>
                    <span className="vital-label">🌡️ TEMP</span>
                    <span className="vital-value">{member.vitals.temperature.toFixed(1)}°C</span>
                  </div>
                  <div className="vital-item">
                    <span className="vital-label">🩸 BP</span>
                    <span className="vital-value">{member.vitals.blood_pressure}</span>
                  </div>
                  <div className="vital-item">
                    <span className="vital-label">☢️ RADIATION</span>
                    <span className="vital-value">{member.radiation_exposure.toFixed(2)} mSv</span>
                  </div>
                  <div className="vital-item">
                    <span className="vital-label">😴 SLEEP (24h)</span>
                    <span className="vital-value">{member.sleep_hours_last_24h.toFixed(1)}h</span>
                  </div>
                  <div className="vital-item">
                    <span className="vital-label">🏃 EXERCISE</span>
                    <span className="vital-value">{member.exercise_compliance ? '✓' : '✗'}</span>
                  </div>
                </div>
              </div>
            );
          })}
        </div>
      </div>
    </div>
  );
}

export default CrewHealth;
