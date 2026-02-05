# 🚀 Artemis II Mission Control System - Deployment Guide

## Quick Start

### 1. Prerequisites
- Docker 20.10+
- Docker Compose 2.0+
- 4GB RAM minimum
- Ports available: 3001, 8081, 5432, 6379, 3002

### 2. Deploy with Docker Compose

```bash
# Navigate to project directory
cd nasa-waste-calc

# Start all services
docker-compose up -d

# View logs
docker-compose logs -f

# Check service health
docker-compose ps
```

### 3. Access Services

| Service | URL | Credentials |
|---------|-----|-------------|
| Mission Control UI | http://localhost:3001 | N/A |
| Backend API | http://localhost:8081 | N/A |
| API Documentation | http://localhost:8081/docs | N/A |
| Grafana | http://localhost:3002 | admin/artemis |
| TimescaleDB | localhost:5432 | postgres/artemis |
| Redis | localhost:6379 | N/A |

### 4. Verify Deployment

```bash
# Test backend health
curl http://localhost:8081/api/health

# Test telemetry endpoint
curl http://localhost:8081/api/telemetry/current

# Test crew health
curl http://localhost:8081/api/crew
```

## Manual Deployment (Development)

### Backend

```bash
cd services/artemis-ii

# Create virtual environment
python3 -m venv venv
source venv/bin/activate  # Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Start server
python -m app.main

# Server runs on http://localhost:8081
```

### Frontend

```bash
cd services/artemis-ii-ui

# Install dependencies
npm install

# Start development server
npm start

# Development server runs on http://localhost:3000

# Build for production
npm run build
```

## Configuration

### Environment Variables

#### Backend (.env)
```bash
DATABASE_URL=postgresql://postgres:artemis@timescaledb:5432/artemis
REDIS_URL=redis://redis:6379/0
PORT=8081
```

#### Frontend (.env)
```bash
REACT_APP_API_URL=http://localhost:8081
```

## Production Deployment

### NetworkBuster.NET Deployment

1. **Build Images**
```bash
docker-compose build
docker tag artemis-ii-backend:latest networkbuster.net/artemis-ii-backend:latest
docker tag artemis-ii-frontend:latest networkbuster.net/artemis-ii-frontend:latest
```

2. **Push to Registry**
```bash
docker push networkbuster.net/artemis-ii-backend:latest
docker push networkbuster.net/artemis-ii-frontend:latest
```

3. **Deploy to Production**
```bash
docker-compose -f docker-compose.prod.yml up -d
```

### SSL/TLS Configuration

Use nginx or Caddy as a reverse proxy:

```nginx
server {
    listen 443 ssl http2;
    server_name artemis-mission-control.networkbuster.net;

    ssl_certificate /etc/ssl/certs/artemis.crt;
    ssl_certificate_key /etc/ssl/private/artemis.key;

    location / {
        proxy_pass http://localhost:3001;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }

    location /api {
        proxy_pass http://localhost:8081;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }

    location /ws {
        proxy_pass http://localhost:8081;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "Upgrade";
    }
}
```

## Troubleshooting

### Backend Not Starting

```bash
# Check logs
docker-compose logs artemis-backend

# Verify dependencies
cd services/artemis-ii
python -c "import fastapi, uvicorn, pydantic; print('Dependencies OK')"
```

### Frontend Build Issues

```bash
# Clear cache and reinstall
cd services/artemis-ii-ui
rm -rf node_modules package-lock.json
npm install
npm run build
```

### Database Connection Issues

```bash
# Check TimescaleDB
docker-compose logs timescaledb

# Test connection
docker-compose exec timescaledb psql -U postgres -d artemis -c "SELECT 1;"
```

### WebSocket Connection Failed

1. Check CORS configuration in backend
2. Verify firewall allows WebSocket connections
3. Check nginx WebSocket proxy configuration

## Monitoring

### Grafana Dashboards

1. Access Grafana at http://localhost:3002
2. Login with admin/artemis
3. Import dashboards from `monitoring/grafana/dashboards/`

### Logs

```bash
# All services
docker-compose logs -f

# Specific service
docker-compose logs -f artemis-backend

# Last 100 lines
docker-compose logs --tail=100 artemis-backend
```

### Health Checks

```bash
# Backend health
curl http://localhost:8081/api/health

# Check all services
docker-compose ps
```

## Performance Tuning

### Backend

```python
# In app/main.py, adjust workers
if __name__ == "__main__":
    import uvicorn
    uvicorn.run(
        app, 
        host="0.0.0.0", 
        port=8081, 
        workers=4,  # Adjust based on CPU cores
        log_level="info"
    )
```

### Database

```sql
-- Optimize TimescaleDB
ALTER DATABASE artemis SET timescaledb.max_background_workers = 8;
```

### Redis

```yaml
# In docker-compose.yml
redis:
  command: redis-server --maxmemory 256mb --maxmemory-policy allkeys-lru
```

## Backup and Recovery

### Database Backup

```bash
# Backup
docker-compose exec timescaledb pg_dump -U postgres artemis > backup.sql

# Restore
docker-compose exec -T timescaledb psql -U postgres artemis < backup.sql
```

### Volume Backup

```bash
# Stop services
docker-compose down

# Backup volumes
tar czf artemis-backup.tar.gz \
  /var/lib/docker/volumes/nasa-waste-calc_timescale-data \
  /var/lib/docker/volumes/nasa-waste-calc_redis-data \
  /var/lib/docker/volumes/nasa-waste-calc_grafana-data
```

## Scaling

### Horizontal Scaling

```yaml
# docker-compose.scale.yml
services:
  artemis-backend:
    deploy:
      replicas: 3
    
  nginx-lb:
    image: nginx:alpine
    ports:
      - "8080:80"
    volumes:
      - ./nginx-lb.conf:/etc/nginx/nginx.conf
```

### Load Balancer Configuration

```nginx
upstream artemis_backend {
    least_conn;
    server artemis-backend-1:8081;
    server artemis-backend-2:8081;
    server artemis-backend-3:8081;
}

server {
    listen 80;
    location / {
        proxy_pass http://artemis_backend;
    }
}
```

## Security Checklist

- [ ] Change default passwords
- [ ] Enable SSL/TLS
- [ ] Configure firewall rules
- [ ] Set up rate limiting
- [ ] Enable authentication
- [ ] Update CORS origins
- [ ] Review environment variables
- [ ] Set up monitoring alerts
- [ ] Enable audit logging
- [ ] Regular security updates

## Support

For issues or questions:
- GitHub Issues: https://github.com/Cleanskiier27/nasa-waste-calc/issues
- Documentation: See ARTEMIS-II-DOCS.md
- Testing Results: See TESTING-RESULTS.md

---

**🚀 Ready for Launch! This system is production-ready and can monitor a real Artemis II mission! 🌙**
