import React from 'react';

function Timeline({ telemetryData }) {
  if (!telemetryData || !telemetryData.timeline) {
    return (
      <div className="timeline panel">
        <div className="panel-header">
          <h3>📅 MISSION TIMELINE</h3>
        </div>
        <div className="panel-content">
          <p>Loading timeline...</p>
        </div>
      </div>
    );
  }

  const { timeline } = telemetryData;

  return (
    <div className="timeline panel">
      <div className="panel-header">
        <h3>📅 MISSION TIMELINE</h3>
      </div>
      <div className="panel-content">
        <div className="current-phase">
          <span className="phase-label">CURRENT PHASE</span>
          <span className="phase-name">{timeline.current_phase}</span>
        </div>

        <div className="timeline-events">
          {timeline.events.map((event, index) => {
            let statusClass = '';
            let statusIcon = '';
            
            if (event.status === 'Complete') {
              statusClass = 'event-complete';
              statusIcon = '✓';
            } else if (event.status === 'In Progress') {
              statusClass = 'event-inprogress';
              statusIcon = '▶';
            } else {
              statusClass = 'event-upcoming';
              statusIcon = '○';
            }

            return (
              <div key={index} className={`timeline-event ${statusClass}`}>
                <div className="event-marker">{statusIcon}</div>
                <div className="event-content">
                  <div className="event-header">
                    <span className="event-met">{event.met}</span>
                    <span className="event-name">{event.event}</span>
                  </div>
                  <p className="event-description">{event.description}</p>
                </div>
              </div>
            );
          })}
        </div>

        {timeline.next_major_event && (
          <div className="next-event">
            <h4>NEXT MAJOR EVENT</h4>
            <div className="next-event-content">
              <span className="next-event-name">{timeline.next_major_event.event}</span>
              <span className="next-event-met">{timeline.next_major_event.met}</span>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}

export default Timeline;
