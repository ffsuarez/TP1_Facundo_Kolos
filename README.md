# Curso Online de Sistemas Embebidos - ARM Book 2024

## Trabajo Práctico N°1: Iluminación y Acceso Controlado

- **Alumno:** Facundo Kolos
- **Email:** fmartinks@gmail.com

### Objetivo
Desarrollar un sistema que permita iluminar un escritorio según la intensidad de luz detectada por un sensor LDR. Además, incorporar un pulsador para accionar una cerradura eléctrica que permite el acceso a una oficina.

### Descripción
El sistema activará una bombilla incandescente mediante un relé cuando la señal del sensor LDR supere un umbral predefinido. También se implementará un pulsador que activará una salida digital para simular la apertura de la cerradura eléctrica.

### Implementación
Inicialmente, debido a la disponibilidad de componentes, se emplearán LEDs en lugar de los componentes finales:
- Un LED simula la bombilla incandescente.
- Otro LED simula la señal de salida para abrir la puerta.

### Plataforma de Desarrollo
NUCLEO F030R8

### Periféricos Utilizados
- **Sensor LDR:** Para medir la intensidad de luz en la oficina.
- **Pulsador:** Para activar la señal de salida que simula la apertura de la puerta.
- **Salida Digital (LED para bombilla incandescente):** Activado cuando la lectura del LDR supera el umbral.
- **Salida Digital (LED para apertura de oficina):** Simula la señal que activa la cerradura eléctrica.

### Estructura del Repositorio
El repositorio contendrá:
- Código fuente para el microcontrolador NUCLEO F030R8.
- Diagramas de conexión y esquemáticos.

### Notas Adicionales
Recientemente, he experimentado dificultades al intentar cargar el programa de ejemplo del curso en mi NUCLEO-F030R8. 
Utilizando MBed OS 2 (la versión compatible con mi placa), intenté compilar "mbed2-example-blinky", el cual compiló correctamente
pero no se cargó en la placa. Aparece un mensaje que indica "PDSC file not found. Flash Failed".
Dicho problema no pudo resolverse, por lo que se optó continuar cargando el binario de forma directa.

---

