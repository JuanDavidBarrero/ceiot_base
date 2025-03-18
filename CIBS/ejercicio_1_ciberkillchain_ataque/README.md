# Cyber Kill Chain - Ataque a Infraestructura LoRaWAN con Raspberry Pi y ESP32

## Escenario  
El atacante busca comprometer el **Raspberry Pi 4**, que actúa como un gateway LoRaWAN, con el objetivo de interceptar, modificar o falsificar la comunicación entre los nodos **ESP32 con SX1307** y el servidor LoRaWAN privado.

## Cyber Kill Chain  

### 1. Reconocimiento (Reconnaissance)  
- **[T1595 - Active Scanning](https://attack.mitre.org/techniques/T1595/):**  
  - Escaneo de redes WiFi o Ethernet para identificar la Raspberry Pi y otros dispositivos en la red.  
  - Uso de `nmap` para detectar puertos abiertos (**22 SSH**, **1700 UDP LoRaWAN**).  
- **[T1590.002 - Gather Victim Network Information](https://attack.mitre.org/techniques/T1590/002/):**  
  - Identificación de servidores LoRaWAN privados.  
  - Análisis de tráfico LoRaWAN con SDR (Software Defined Radio).  

### 2. Armamento (Weaponization)  
- **[T1203 - Exploitation for Client Execution](https://attack.mitre.org/techniques/T1203/):**  
  - Desarrollo de exploits dirigidos al software de **LoRa Packet Forwarder**.  
- **[T1566 - Spearphishing](https://attack.mitre.org/techniques/T1566/):**  
  - Envío de correos de phishing a administradores para obtener credenciales SSH.  

### 3. Entrega (Delivery)  
- **[T1071.001 - Application Layer Protocol (Web Protocols)](https://attack.mitre.org/techniques/T1071/001/):**  
  - Uso de un servidor web malicioso para entregar un payload que explote vulnerabilidades en Debian o en la configuración del gateway.  
- **[T1104 - Multi-Stage Channels](https://attack.mitre.org/techniques/T1104/):**  
  - Explotación combinada de SSH y LoRa para obtener acceso persistente.  

### 4. Explotación (Exploitation)  
- **[T1133 - External Remote Services](https://attack.mitre.org/techniques/T1133/):**  
  - Ataques de fuerza bruta o credenciales predeterminadas (`usuario: pi`) para acceder por **SSH (puerto 22)**.  
- **[T1210 - Exploitation of Remote Services](https://attack.mitre.org/techniques/T1210/):**  
  - Explotación de vulnerabilidades en **LoRa Packet Forwarder** para tomar control del gateway.  

### 5. Instalación (Installation)  
- **[T1547 - Boot or Logon Autostart Execution](https://attack.mitre.org/techniques/T1547/):**  
  - Modificación de archivos de arranque en la Raspberry Pi para ejecutar código malicioso en cada reinicio.  
- **[T1072 - Software Deployment Tools](https://attack.mitre.org/techniques/T1072/):**  
  - Uso de herramientas legítimas (`apt`, `wget`) para descargar malware persistente.  

### 6. Comando y Control (Command & Control - C2)  
- **[T1573 - Encrypted Channel](https://attack.mitre.org/techniques/T1573/):**  
  - Establecimiento de un túnel SSH inverso para mantener control remoto sobre la Raspberry Pi.  
- **[T1102 - Web Service](https://attack.mitre.org/techniques/T1102/):**  
  - Uso de servicios como Telegram o Discord para recibir comandos.  

### 7. Acciones sobre Objetivos (Actions on Objectives)  
- **[T1020 - Automated Exfiltration](https://attack.mitre.org/techniques/T1020/):**  
  - Captura de paquetes LoRa y envío a un servidor de ataque para análisis.  
- **[T1485 - Data Destruction](https://attack.mitre.org/techniques/T1485/):**  
  - Eliminación de logs en la Raspberry Pi para ocultar el ataque.  
- **[T1496 - Resource Hijacking](https://attack.mitre.org/techniques/T1496/):**  
  - Uso de la Raspberry Pi como **relay malicioso** para interceptar y modificar la comunicación LoRa.
