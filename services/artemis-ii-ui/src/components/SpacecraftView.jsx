import React, { useRef } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import { OrbitControls, Stars, Sphere } from '@react-three/drei';
import * as THREE from 'three';

function Earth() {
  const earthRef = useRef();

  useFrame(() => {
    if (earthRef.current) {
      earthRef.current.rotation.y += 0.001;
    }
  });

  return (
    <Sphere ref={earthRef} args={[1, 32, 32]} position={[0, 0, 0]}>
      <meshStandardMaterial
        color="#1e90ff"
        roughness={0.7}
        metalness={0.3}
      />
    </Sphere>
  );
}

function Moon() {
  return (
    <Sphere args={[0.27, 16, 16]} position={[6, 0, 0]}>
      <meshStandardMaterial
        color="#cccccc"
        roughness={0.9}
        metalness={0.1}
      />
    </Sphere>
  );
}

function Spacecraft({ position }) {
  const spacecraftRef = useRef();

  // Convert position from km to scene units (Earth radius = 1)
  const EARTH_RADIUS = 6371; // km
  const scenePosition = [
    position.x / EARTH_RADIUS,
    position.y / EARTH_RADIUS,
    position.z / EARTH_RADIUS
  ];

  return (
    <group ref={spacecraftRef} position={scenePosition}>
      <Sphere args={[0.05, 8, 8]}>
        <meshStandardMaterial
          color="#ff6600"
          emissive="#ff3300"
          emissiveIntensity={0.5}
        />
      </Sphere>
      {/* Add a small trail indicator */}
      <mesh position={[0, 0, -0.1]}>
        <coneGeometry args={[0.02, 0.1, 8]} />
        <meshStandardMaterial
          color="#ffffff"
          emissive="#ffffff"
          emissiveIntensity={0.3}
        />
      </mesh>
    </group>
  );
}

function TrajectoryPath({ positions }) {
  if (!positions || positions.length < 2) return null;

  const points = positions.map(pos => {
    const EARTH_RADIUS = 6371;
    return new THREE.Vector3(
      pos.x / EARTH_RADIUS,
      pos.y / EARTH_RADIUS,
      pos.z / EARTH_RADIUS
    );
  });

  const curve = new THREE.CatmullRomCurve3(points);
  const curvePoints = curve.getPoints(100);
  const geometry = new THREE.BufferGeometry().setFromPoints(curvePoints);

  return (
    <line geometry={geometry}>
      <lineBasicMaterial color="#00ff00" linewidth={2} />
    </line>
  );
}

function SpacecraftView({ telemetryData }) {
  const position = telemetryData?.spacecraft?.position || { x: 6571, y: 0, z: 0 };
  
  // Generate a simple trajectory for visualization
  const trajectoryPositions = [];
  for (let i = 0; i < 50; i++) {
    const angle = (i / 50) * Math.PI * 0.3;
    const distance = 6571 + i * 7500;
    trajectoryPositions.push({
      x: distance * Math.cos(angle),
      y: distance * Math.sin(angle),
      z: distance * 0.05
    });
  }

  return (
    <div style={{ width: '100%', height: '100%', background: '#000' }}>
      <Canvas camera={{ position: [0, 5, 10], fov: 50 }}>
        <ambientLight intensity={0.3} />
        <pointLight position={[10, 10, 10]} intensity={1.5} />
        <Stars radius={300} depth={50} count={5000} factor={4} fade speed={1} />
        
        <Earth />
        <Moon />
        <Spacecraft position={position} />
        <TrajectoryPath positions={trajectoryPositions} />
        
        <OrbitControls 
          enablePan={true}
          enableZoom={true}
          enableRotate={true}
          minDistance={2}
          maxDistance={50}
        />
      </Canvas>
    </div>
  );
}

export default SpacecraftView;
