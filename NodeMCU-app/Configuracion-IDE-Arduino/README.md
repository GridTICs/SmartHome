# Introducción

Para poder utilizar el Arduino IDE con la placa NodeMCU primero deberemos realizarle al Arduino IDE algunas configuraciones adicionales.
Además debido a que necesitamos una comunicación cifrada necesitamos adjuntar los certificados en nuestro código. Esto se logra a través de la librería FS.h.
Para que esta librería pueda actuar deberemos agregar a nuestro IDE Arduino una herramienta especial.
En esta sección se detallan ambas configuraciones. El IDE Arduino deberá ser una versión mayor a la 1.6.9.

## Intalar Arduino IDE para ESP8266

Una vez instalado el Arduino IDE. Deberemos dirigirnos a "File > Preferences" tal como indica la siguiente figura.

![file-preferences](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/file-preferences.png)

Se abrirá una ventana llamada "Preferences". En el cuadro en blanco que dice "Additional Boards Manager URLs" colocamos lo siguiente:

```
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

![preferences](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/preferences.png)

Luego se selecciona "Tools > Board: > Boards Manager..."

![tools-board](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/tools-board.png)

Aparecera una ventana llamada "Boards Manager". En el cuadro de busqueda debera tipear "esp8266". Seleccione "ESP8266 Community".
Luego "Install". Despues de unos minutos se instalará. Luego cierre la ventana.

![board-manager](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/board-manager.png)

Ahora haga click sobre "Tools > Board" y veremos que aparecen las placas "ESP8266 Boards". Seleccionamos "NodeMCU 1.0"

![Board-final](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/Board-final.png)

## SPIFFS (SPI Flash File System)

SPIFFS (SPI Flash File System) es un sistema de archivo diseñado para sistemas embebidos con poco memoria.
Mediante este sistema se pueden cargar en la memoria de las placas tanto los programas como los archivos utilizados para realizar la conexion encriptada.
Para usar cualquiera de las funciones de este sistema de archivo en un sketch arduino deberemos incluir la libreria FS.h

```
#include "FS.h"
```

### Instalar el cargador del sistema de archivos.

Para instalar el cargador del sistema de archivos primero deberemos descargar el archivo esp8266fs.jar.
Al instalar el IDE Arduino se crea automaticamente una carpeta llamada Arduino. En esta ultima existe una subcarpeta llamada "libraries".


![Aquí la descripción de la imagen por si no carga](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/arduino-libraries.png)

Dentro de la carpeta Arduino, antes mencionada, crearemos una carpeta llamada "tools". A su vez dentro de esta crearemos otra carpeta llamada "ESP8266". Que contendra una carpeta llamada "tool". En esta última colocaremos el archivo "esp8266fs.jar".

![Aquí la descripción de la imagen por si no carga](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/esp8266fs-jar.png)

### Ingreso de los archivos certificados a la placa.

En la misma carpeta de nuestro proyecto colocaremos los archivos utilizados para la comunicación encriptada.
Para ello crearemos una carpeta llamada "data".

![Aquí la descripción de la imagen por si no carga](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/carpeta-proyecto.png)

Dentro de esta carpeta colocaremos los archivos con los certificados. 

![Aquí la descripción de la imagen por si no carga](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/certificados.png)

Luego en el IDE Arduino nos dirigiremos a la seccion tool -> ESP8266 Sketch Data Upload.

![Aquí la descripción de la imagen por si no carga](https://github.com/GridTICs/SmartHome/blob/master/NodeMCU-app/Configuracion-IDE-Arduino/ESP8266_Sketch_Data_Upload.png)

Al clickear el Arduino IDE comenzara la subida de los archivos que se encuentran en la carpeta data a la placa.
Deberemos esperar un tiempo hasta que nos diga que se completo en un 100% y listo.
Debemos tener en cuenta que la carga de los archivos se debe realizar despues de la carga del sketch. Ademas la placa debe estar conectada para realizar la carga de los certificados.
