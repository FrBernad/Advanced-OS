# TP2 - Advanced OS

# Authors
- [Francisco Bernad](https://github.com/FrBernad)
- [Nicolás Rampoldi](https://github.com/NicolasRampoldi) 


# Manual de usuario

1. Ingresar a la carpeta ***"Toolchain"*** y ejecutar el comando `make all`.

2. En la carpeta principal, ejecutar el comando `make all`.

3. Ejecutar el programa mediante terminal ingresando `./run.sh` en caso de linux o `./run.ps1` en caso de Windows.

Una vez ejecutado el programa, se abrirá una ventana donde se estará ejecutando la ***shell***. 


## Shell

La shell brinda una variedad de comandos, lo cuales son ejecutados al escribir su nombre y presionar ***enter***.
Los comandos son los siguientes:

- ***help:*** muestra la lista de comandos y la descripción de cada uno.
- ***inforeg:*** imprime en pantalla el valor de los registros del programa en cierto momento, para actualizar dicho valor presionar ctrl + s.
- ***time:*** imprime en pantalla el tiempo actual del sistema.
- ***cpuInfo:*** imprime en pantalla el modelo y vendedor del CPU. 
- ***cpuTemp:*** imprime en pantalla la temperatura actual del CPU. 
- ***printmem:*** dump de 32 bytes de memoria a partir de la dirección hexadecimal pasada como parámetro. 
- ***checkZeroException:*** causa una excepción de división por cero. 
- ***checkInvalidOpCodeException:*** causa una excepción de código de operación inválido.
- ***changeUsername:*** cambia el nombre del usuario de la shell al pasado como parámetro.
- ***showArgs:*** imprime en pantalla los argumentos del comando.
- ***ps*** reporta el estado de los procesos.
- ***loop*** crea un proceso el cual loopea.
- ***kill*** mata el proceso cuyo pid es el recibido como argumento.
- ***nice*** cambia la prioridad del proceso cuyo pid es el recibido como argumento.
- ***block*** bloquea el proceso cuyo pid es el recibido como argumento.
- ***unblock*** desbloquea el proceso cuyo pid es el recibido como argumento.
- ***testProcesses*** prueba la creacion de procesos con el scheduler.
- ***testPriority*** prueba la prioridad del scheduler.
- ***testSync*** prueba la sincronizacion de semaforos (debe funcionar).
- ***testNoSync*** prueba la sincronizacion de semaforos (debe fallar).
- ***testMM*** prueba el memory manager.
- ***pipe*** dump de los pipes activos.
- ***sem*** dump de los semaforos activos.
- ***wc*** cuenta las lineas recibidas como argumento.
- ***filter***  filtra las vocales recibidas como argumento.
- ***cat*** imprime en pantalla los caracteres recibidos como argumento.
- ***phylo*** problema de los filosofos comiendo, recibe el numero inicial de filosofos como argumento (min 2, max 6).

### Teclas especiales

- ***ctr + l:*** limpia la pantalla.
- ***ctr + c:*** mata al proceso que se encuentre corriendo en foreground.
- ***ctr + d:*** manda la señal de EOF.

### Comportamientos especiales
Para correr un programa en **background** ejecute `commandName &`.

Para **pipear** la salida de un comando a la entrada de otro ejecute `commandName1 | commandName2`.

## Syscalls

***int 80h***

| %rax | System Call        | %rdi                            | %rsi                  | %rdx              | %r10                | %r8           | %r9 |
| ---- | ------------------ | ------------------------------- | --------------------- | ----------------- | ------------------- | ------------- | --- |
| 0    | sys_getMem         | uint64_t memDir                 | uint64_t* memData     |                   |                     |               |     |
| 1    | sys_rtcTime        | t_timeInfo tInfo                |                       |                   |                     |               |     |
| 2    | sys_temp           |                                 |                       |                   |                     |               |     |
| 3    | sys_write          | char * string                   | uint8_t length        | t_colour bgColour | t_colour fontColour |               |     |
| 4    | sys_getchar        |                                 |                       |                   |                     |               |     |
| 5    | sys_clear          |                                 |                       |                   |                     |               |     |
| 6    | sys_loadApp        | void (*entryPoint)(int, char**) | int argc              | char** argv       | uint8_t fg          | uint16_t * fd |     |
| 7    | sys_inforeg        |                                 |                       |                   |                     |               |     |
| 8    | sys_ps             |                                 |                       |                   |                     |               |     |
| 9    | sys_secsElapsed    |                                 |                       |                   |                     |               |     |
| 10   | sys_kill           | uint64_t pid                    |                       |                   |                     |               |     |
| 11   | sys_nice           | uint64_t pid                    | uint64_t priority     |                   |                     |               |     |
| 12   | sys_block          | uint64_t pid                    |                       |                   |                     |               |     |
| 13   | sys_unblock        | uint64_t pid                    |                       |                   |                     |               |     |
| 14   | sys_getPID         |                                 |                       |                   |                     |               |     |
| 15   | sys_malloc         | uint32_t nbytes                 |                       |                   |                     |               |     |
| 16   | sys_free           | void* ptr                       |                       |                   |                     |               |     |
| 17   | sys_yield          |                                 |                       |                   |                     |               |     |
| 18   | sys_sem_open       | char* name                      | uint64_t initialCount |                   |                     |               |     |
| 19   | sys_sem_close      | int semIndex                    |                       |                   |                     |               |     |
| 20   | sys_sem_post       | int semIndex                    |                       |                   |                     |               |     |
| 21   | sys_sem_wait       | int semIndex                    |                       |                   |                     |               |     |
| 22   | sys_dumpSemaphores |                                 |                       |                   |                     |               |     |
| 23   | sys_popen          | char* pipeName                  |                       |                   |                     |               |     |
| 24   | sys_closePipe      | int pipeIndex                   |                       |                   |                     |               |     |
| 25   | sys_writePipe      | int pipeIndex                   | char* string          |                   |                     |               |     |
| 26   | sys_readPipe       | int pipeIndex                   |                       |                   |                     |               |     |
| 27   | sys_dumpPipes      |                                 |                       |                   |                     |               |     |
| 28   | sys_wait           | uint64_t pid                    |                       |                   |                     |               |     |
| 29   | sys_ticksElapsed   |                                 |                       |                   |                     |               |     |