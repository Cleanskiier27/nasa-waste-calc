import React, { useState, useEffect } from 'react';

function AlertPanel({ telemetryData }) {
  const [alerts, setAlerts] = useState([]);

  useEffect(() => {
    // Generate sample alerts for demonstration
    const sampleAlerts = [
      {
        id: 1,
        severity: 'INFO',
        system: 'COMMUNICATIONS',
        message: 'DSN Goldstone contact acquired',
        timestamp: new Date(Date.now() - 300000).toISOString(),
        acknowledged: false
      }
    ];
    setAlerts(sampleAlerts);
  }, [telemetryData]);

  const getSeverityClass = (severity) => {
    switch (severity) {
      case 'CRITICAL':
        return 'alert-critical';
      case 'CAUTION':
        return 'alert-caution';
      case 'WARNING':
        return 'alert-warning';
      case 'INFO':
      default:
        return 'alert-info';
    }
  };

  const getSeverityIcon = (severity) => {
    switch (severity) {
      case 'CRITICAL':
        return '🚨';
      case 'CAUTION':
        return '⚠️';
      case 'WARNING':
        return '⚡';
      case 'INFO':
      default:
        return 'ℹ️';
    }
  };

  const acknowledgeAlert = (id) => {
    setAlerts(alerts.map(alert => 
      alert.id === id ? { ...alert, acknowledged: true } : alert
    ));
  };

  const activeAlerts = alerts.filter(a => !a.acknowledged);
  const acknowledgedAlerts = alerts.filter(a => a.acknowledged);

  return (
    <div className="alert-panel panel">
      <div className="panel-header">
        <h3>🔔 ALERTS & NOTIFICATIONS</h3>
        <span className="alert-count">
          {activeAlerts.length > 0 ? `${activeAlerts.length} Active` : 'All Clear'}
        </span>
      </div>
      <div className="panel-content">
        {activeAlerts.length === 0 && acknowledgedAlerts.length === 0 && (
          <div className="no-alerts">
            <p>✓ All systems nominal</p>
            <p className="no-alerts-subtitle">No active alerts</p>
          </div>
        )}

        {activeAlerts.length > 0 && (
          <div className="alerts-section">
            <h4>Active Alerts</h4>
            {activeAlerts.map(alert => (
              <div key={alert.id} className={`alert-item ${getSeverityClass(alert.severity)}`}>
                <div className="alert-header">
                  <span className="alert-icon">{getSeverityIcon(alert.severity)}</span>
                  <span className="alert-severity">{alert.severity}</span>
                  <span className="alert-system">{alert.system}</span>
                </div>
                <p className="alert-message">{alert.message}</p>
                <div className="alert-footer">
                  <span className="alert-time">
                    {new Date(alert.timestamp).toLocaleTimeString()}
                  </span>
                  <button 
                    className="alert-ack-btn"
                    onClick={() => acknowledgeAlert(alert.id)}
                  >
                    Acknowledge
                  </button>
                </div>
              </div>
            ))}
          </div>
        )}

        {acknowledgedAlerts.length > 0 && (
          <div className="alerts-section acknowledged-section">
            <h4>Acknowledged</h4>
            {acknowledgedAlerts.slice(0, 3).map(alert => (
              <div key={alert.id} className="alert-item alert-acknowledged">
                <div className="alert-header">
                  <span className="alert-icon">✓</span>
                  <span className="alert-severity">{alert.severity}</span>
                  <span className="alert-system">{alert.system}</span>
                </div>
                <p className="alert-message">{alert.message}</p>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}

export default AlertPanel;
