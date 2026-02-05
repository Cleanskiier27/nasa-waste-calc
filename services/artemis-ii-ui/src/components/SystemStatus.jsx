import React from 'react';

function formatNumber(num, decimals = 2) {
  if (num === null || num === undefined) return 'N/A';
  return Number(num).toFixed(decimals);
}

function getStatusClass(value, nominal, tolerance = 0.1) {
  const lower = nominal * (1 - tolerance);
  const upper = nominal * (1 + tolerance);
  if (value < lower || value > upper) return 'status-warning';
  return 'status-nominal';
}

function SystemStatus({ telemetryData }) {
  if (!telemetryData) {
    return (
      <div className="system-status panel">
        <div className="panel-header">
          <h3>⚙️ SYSTEMS STATUS</h3>
        </div>
        <div className="panel-content">
          <p>Loading systems data...</p>
        </div>
      </div>
    );
  }

  const { eclss, power, propulsion, communications } = telemetryData;

  return (
    <div className="system-status panel">
      <div className="panel-header">
        <h3>⚙️ SYSTEMS STATUS</h3>
      </div>
      <div className="panel-content">
        <div className="systems-grid">
          {/* ECLSS */}
          <div className="system-card">
            <h4>🫁 ECLSS (Life Support)</h4>
            <div className="system-params">
              <div className={`param-item ${getStatusClass(eclss.oxygen.partial_pressure, 160, 0.02)}`}>
                <span className="param-label">O₂ Partial Pressure</span>
                <span className="param-value">{formatNumber(eclss.oxygen.partial_pressure, 1)} mmHg</span>
              </div>
              <div className={`param-item ${getStatusClass(eclss.co2_removal.co2_level, 3.0, 0.2)}`}>
                <span className="param-label">CO₂ Level</span>
                <span className="param-value">{formatNumber(eclss.co2_removal.co2_level, 2)} mmHg</span>
              </div>
              <div className="param-item">
                <span className="param-label">Scrubber Status</span>
                <span className="param-value">{eclss.co2_removal.scrubber_status}</span>
              </div>
              <div className={`param-item ${getStatusClass(eclss.cabin.pressure, 14.7, 0.03)}`}>
                <span className="param-label">Cabin Pressure</span>
                <span className="param-value">{formatNumber(eclss.cabin.pressure, 2)} PSI</span>
              </div>
              <div className="param-item">
                <span className="param-label">Cabin Temp</span>
                <span className="param-value">{formatNumber(eclss.cabin.temperature, 1)}°C</span>
              </div>
              <div className="param-item status-nominal">
                <span className="param-label">Air Quality</span>
                <span className="param-value">{eclss.cabin.air_quality_index}/100</span>
              </div>
            </div>
          </div>

          {/* Power System */}
          <div className="system-card">
            <h4>⚡ POWER SYSTEM</h4>
            <div className="system-params">
              <div className="param-item status-nominal">
                <span className="param-label">Solar Array Output</span>
                <span className="param-value">{formatNumber(power.solar_arrays.output_current, 2)} kW</span>
              </div>
              <div className="param-item">
                <span className="param-label">Sun Angle</span>
                <span className="param-value">{formatNumber(power.solar_arrays.sun_angle, 1)}°</span>
              </div>
              <div className="param-item">
                <span className="param-label">Power Consumption</span>
                <span className="param-value">{formatNumber(power.power_consumption.total, 2)} kW</span>
              </div>
              <div className={`param-item ${power.power_budget.margin > 10 ? 'status-nominal' : 'status-warning'}`}>
                <span className="param-label">Power Margin</span>
                <span className="param-value">{formatNumber(power.power_budget.margin, 1)}%</span>
              </div>
              <div className="param-item">
                <span className="param-label">Battery Avg SOC</span>
                <span className="param-value">
                  {formatNumber(power.batteries.reduce((sum, b) => sum + b.state_of_charge, 0) / power.batteries.length, 1)}%
                </span>
              </div>
            </div>
          </div>

          {/* Propulsion */}
          <div className="system-card">
            <h4>🚀 PROPULSION</h4>
            <div className="system-params">
              <div className="param-item">
                <span className="param-label">Main Engine</span>
                <span className="param-value">{propulsion.main_engine.status}</span>
              </div>
              <div className="param-item">
                <span className="param-label">Hydrazine</span>
                <span className="param-value">{formatNumber(propulsion.fuel.hydrazine, 0)} kg</span>
              </div>
              <div className="param-item">
                <span className="param-label">MON Oxidizer</span>
                <span className="param-value">{formatNumber(propulsion.fuel.mon_oxidizer, 0)} kg</span>
              </div>
              <div className="param-item status-nominal">
                <span className="param-label">Delta-V Available</span>
                <span className="param-value">{formatNumber(propulsion.fuel.total_delta_v, 0)} m/s</span>
              </div>
              <div className="param-item">
                <span className="param-label">Delta-V Used</span>
                <span className="param-value">{formatNumber(propulsion.fuel.used_delta_v, 0)} m/s</span>
              </div>
              <div className="param-item">
                <span className="param-label">Active RCS</span>
                <span className="param-value">
                  {propulsion.rcs_thrusters.filter(t => t.status === 'Active').length}/24
                </span>
              </div>
            </div>
          </div>

          {/* Communications */}
          <div className="system-card">
            <h4>📡 COMMUNICATIONS</h4>
            <div className="system-params">
              {communications.dsn_contacts.map((contact, idx) => (
                <React.Fragment key={idx}>
                  <div className="param-item status-nominal">
                    <span className="param-label">DSN Station</span>
                    <span className="param-value">{contact.station}</span>
                  </div>
                  <div className="param-item">
                    <span className="param-label">Signal Strength</span>
                    <span className="param-value">{formatNumber(contact.signal_strength, 1)} dBm</span>
                  </div>
                  <div className="param-item">
                    <span className="param-label">Data Rate</span>
                    <span className="param-value">{formatNumber(contact.data_rate, 2)} Mbps</span>
                  </div>
                </React.Fragment>
              ))}
              <div className="param-item">
                <span className="param-label">Signal Delay</span>
                <span className="param-value">{formatNumber(communications.current_downlink.delay, 2)}s</span>
              </div>
              <div className="param-item">
                <span className="param-label">Video Feed</span>
                <span className="param-value">{communications.current_downlink.video ? '✓' : '✗'}</span>
              </div>
              <div className="param-item">
                <span className="param-label">Voice Loop</span>
                <span className="param-value">{communications.current_downlink.voice ? '✓' : '✗'}</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}

export default SystemStatus;
