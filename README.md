# Nurse-Scheduling_Problem [C++ Version]


Proyecto Final de INF-295 Inteligencia Artificial USM (2021-1)  
Profesora **Maria-Cristina Riff**

El objetivo del problema es encontrar un *horario factible* donde se asignan enfermeras/os a los distintos turnos dentro del horizonte de planeación. Se deben cumplir las *restricciones fuertes*, mientras que buscamos respetar la mayor cantidad de *restricciones blandas*, de tal manera de encontrar un horario que satisfaga a los empleados.


## NSP
NSP es un problema del tipo *SCHEDULING* ...

## Ejecución

Antes de ejecutar el algoritmo se debe compilar todo el proyecto ejecutando:
```
$ make all
```   
Una vez compilado todo, se tiene que ejecutar con la siguiente instrucción
```
$ ./NSP <ruta_instancia> <numero_max_ciclo>
```
**Donde**:  
` <ruta_instancia> `    : Ruta en donde se encuentra la instancia que se desea evaluar.   
` <numero_max_ciclo> `  : Número Máximo el cual el algorimo ejecutará el ciclo.   



Para limpiar todos los archivos generados pos ejecución se debe ejecutar
```
$ make clear
```
## Instancias
Son archivos que contienen todos los parámetros necesarios para que la F.O. del problema
encuentre la(s) solución(es)
### Modificación de instancias
En las instancias 10 y 12 se debe modificar los indentificadores de cada turno a char de largo 1 para que el algoritmo planifique sin problema.