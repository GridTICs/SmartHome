# Introducción

Para poder poder realizar la comunicación cifrada necesitamos adjuntar los certificados en nuestro código.
Esto se logra a traves de la libreria FS.h. Para que esta libreria pueda actuar deberemos agregar a nuestro IDE Arduino una herramienta especial. El IDE Arduino debera ser una version mayor a la 1.6.9.

## SPIFFS (SPI Flash File System)

SPIFFS (SPI Flash File System) es un sistema de archivo diseñado para sistemas embebidos con poco memoria.
Mediante este sistema se pueden cargar en la memoria de las placas tanto los programas como los archivos utilizados para realizar la conexion encriptada.
Para usar cualquiera de las funciones de este sistema de archivo en un sketch arduino deberemos incluir la libreria FS.h

#include "FS.h"

### Instalar el cargador del sistema de archivos.

Para instalar el cargador del sistema de archivos primero deberemos descargar el archivo esp8266fs.jar.
Al instalar el IDE Arduino se crea automaticamente una carpeta llamada Arduino. En esta ultima existe una subcarpeta llamada "libraries".
Dentro de la carpeta Arduino, antes mencionada, crearemos una carpeta llamada "tools". A su vez dentro de esta crearemos otra carpeta llamada "ESP8266". Que contendra una carpeta llamada "tool". En esta última colocaremos el archivo "esp8266fs.jar".

### Ingreso de los archivos certificados a la placa.

En la misma carpeta de nuestro proyecto colocaremos los archivos utilizados para la comunicación encriptada. Para ello crearemos una carpeta llamada "data". Dentro de esta carpeta colocaremos los archivos con los certificados. 
Luego en el IDE Arduino nos dirigiremos a la seccion tool -> ESP8266 Sketch Data Upload.
Iremos subiendo los archivos uno por uno.
Debemos tener en cuenta que la carga de los archivos se debe realizar despues de la carga del sketch. Ademas la placa debe estar conectada para realizar la carga de los certificados.

