import React, { useState, useEffect } from 'react';
import MissionControl from './components/MissionControl';
import './styles/mission-control.css';

function App() {
  const [connected, setConnected] = useState(false);

  return (
    <div className="App">
      <MissionControl />
    </div>
  );
}

export default App;
