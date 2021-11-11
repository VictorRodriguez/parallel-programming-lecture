Homework: lab 06: milk server and milk seller

Programa: milk-server-A00827629.c
Realiza la tarea de inializar el espacio de memoria compartida con el número básico de 
default de botellas de leche. El servidor no se detiene hasta que el usuario presione 1 que
es la opción de salida.

Programa: milk-client-A00827629.c
Al correr el cliente se despliega el menú de opciones disponibles. 
Con 0 el usuario sale del programa y termina
Con 1 el usuario consulta el valor actual de botella disponibles
Con 2 el usuario realiza la venta y resta al número de botellas disponibles
El programa notifica al usuario que no se pueden vender más botellas cuando el valor es 0
Mediante semáforos, se limita el acceso al recurso de número de botellas.

Compilación:
gcc milk-server-A00827629.c -o milk-server-A00827629 lpthread -lm
gcc milk-client-A00827629.c -o milk-client-A00827629 lpthread -lm