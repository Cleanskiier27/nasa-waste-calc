import { useState, useEffect, useRef } from 'react';

const API_URL = process.env.REACT_APP_API_URL || 'http://localhost:8081';
const WS_URL = API_URL.replace('http', 'ws');

export const useTelemetry = () => {
  const [telemetryData, setTelemetryData] = useState(null);
  const [connected, setConnected] = useState(false);
  const [error, setError] = useState(null);
  const ws = useRef(null);

  useEffect(() => {
    // Initialize WebSocket connection
    const connectWebSocket = () => {
      try {
        ws.current = new WebSocket(`${WS_URL}/ws/telemetry`);

        ws.current.onopen = () => {
          console.log('WebSocket connected');
          setConnected(true);
          setError(null);
        };

        ws.current.onmessage = (event) => {
          try {
            const data = JSON.parse(event.data);
            setTelemetryData(data);
          } catch (err) {
            console.error('Error parsing telemetry data:', err);
          }
        };

        ws.current.onerror = (err) => {
          console.error('WebSocket error:', err);
          setError('WebSocket connection error');
          setConnected(false);
        };

        ws.current.onclose = () => {
          console.log('WebSocket disconnected');
          setConnected(false);
          
          // Attempt to reconnect after 5 seconds
          setTimeout(() => {
            connectWebSocket();
          }, 5000);
        };
      } catch (err) {
        console.error('Failed to create WebSocket:', err);
        setError('Failed to create WebSocket connection');
      }
    };

    connectWebSocket();

    // Cleanup on unmount
    return () => {
      if (ws.current) {
        ws.current.close();
      }
    };
  }, []);

  return { telemetryData, connected, error };
};

export default useTelemetry;
