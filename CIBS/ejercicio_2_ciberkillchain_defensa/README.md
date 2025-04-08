
# Defensa Contra Ataques en Infraestructuras LoRaWAN

Este documento describe posibles ataques a infraestructuras basadas en LoRaWAN y cómo defenderse utilizando el modelo **Cyber Kill Chain**. También se incluyen imágenes sugeridas para cada fase y fragmentos de código útil para implementar medidas de seguridad.

---

## 1. Reconocimiento (Reconnaissance)

En esta fase, el atacante recolecta información del objetivo, como direcciones IP públicas, servicios expuestos o dispositivos conectados (por ejemplo, gateways LoRaWAN).

![Escaneo de red de gateway LoRaWAN](https://tse3.mm.bing.net/th?id=OIP.yDcNpLjJqSKuYYcelxf4QgHaEd&pid=Api)

### Medidas de defensa

- **Firewall** para bloquear escaneos de red comunes:

```bash
# Bloquear intentos de escaneo de puertos
iptables -A INPUT -p tcp --tcp-flags ALL NONE -j DROP
iptables -A INPUT -p tcp ! --syn -m state --state NEW -j DROP
```

---

## 2. Armamento (Weaponization)

El atacante crea una carga maliciosa diseñada para explotar una vulnerabilidad específica del gateway o red.

![Creación de malware para IoT](https://tse3.mm.bing.net/th?id=OIP.KIShdpMpEWUDlkaCb8Rm-gHaE_&pid=Api)

### Medidas de defensa

- Usar herramientas como `rkhunter` para detectar rootkits o malware en el sistema:

```bash
sudo apt-get install rkhunter
sudo rkhunter --check
```

---

## 3. Entrega (Delivery)

El atacante entrega el malware a través de phishing, USBs infectados o servicios expuestos como SSH.

![Phishing a administrador LoRaWAN](https://tse1.mm.bing.net/th?id=OIP.RlEaZSYVsHyw0cYb82HjAgHaGW&pid=Api)

### Medidas de defensa

- Instalar y configurar `fail2ban` para proteger SSH contra fuerza bruta:

```bash
sudo apt-get install fail2ban

sudo tee /etc/fail2ban/jail.local > /dev/null <<EOL
[sshd]
enabled = true
port = 22
filter = sshd
logpath = /var/log/auth.log
maxretry = 5
EOL

sudo systemctl restart fail2ban
```

---

## 4. Explotación (Exploitation)

Una vez que el malware llega al sistema, se explota una vulnerabilidad para obtener control.

### Medidas de defensa

- Mantener el sistema actualizado:

```bash
sudo apt-get update
sudo apt-get upgrade
```

---

## 5. Instalación (Installation)

El atacante instala software malicioso para obtener persistencia (backdoors, scripts).

### Imagen sugerida

Un diagrama de instalación de rootkit/backdoor silencioso tras explotación.

### Medidas de defensa

- Verificar archivos de inicio y cronjobs sospechosos:

```bash
cat /etc/rc.local
crontab -l
```

---

## 6. Comando y Control (C2)

El malware se comunica con el servidor del atacante para recibir instrucciones.

### Imagen sugerida

Diagrama mostrando conexión entre el gateway y un servidor externo de C2.

### Medidas de defensa

- Revisar conexiones salientes sospechosas:

```bash
netstat -tnp
```

- Usar `ufw` o `iptables` para limitar tráfico saliente a puertos específicos.

---

## 7. Acciones sobre Objetivos (Actions on Objectives)

Una vez dentro, el atacante puede alterar datos, interrumpir servicios o exfiltrar información.

![Manipulación de paquetes LoRaWAN](https://tse3.mm.bing.net/th?id=OIP.KIShdpMpEWUDlkaCb8Rm-gHaE_&pid=Api)

### Medidas de defensa

- Monitorizar el tráfico de red:

```bash
# Capturar paquetes LoRaWAN (puerto 1700)
sudo tcpdump -i wlan0 port 1700
```

- Asegurar integridad y cifrado de datos con TLS y autenticación mutua.

---

## Recomendaciones Generales

- Deshabilitar servicios innecesarios en el gateway.
- Usar autenticación por clave pública para SSH.
- Registrar logs de acceso e integrarlos con un SIEM si es posible.
- Establecer listas blancas de IP si el gateway se administra remotamente.

---

