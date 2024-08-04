# Antes de ejecutar el dispositivo, asegúrate de realizar los siguientes pasos:

1. Ejecuta el contenedor de MongoDB:
   ```
   docker run -p 27017:27017 -v /home/iiot/ceiot_base/api/volume:/data/db mongo:4.0.4


   ```

2. Accede al directorio de la API y ejecuta el servidor:

    ```
   cd ~/ceiot_base/api
   node index.js
   ```

3. Accede al directorio de la SPA y reconstruye el proyecto:

    ```
   cd ~/ceiot_base/api/spa
   ./rebuild.sh
   ```

    
device_id_mac  done
to_json  done
persistence_real  done 

