#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ctype.h>

#define PUERTO 8081
#define TAM_BUFFER 1024

int es_numero(const char *str)
{
    // Verifica si la cadena está vacía
    if (*str == '\0')
    {
        return 0;
    }

    // Verifica cada carácter en la cadena
    while (*str)
    {
        if (!isdigit((unsigned char)*str))
        {
            return 0; // No es un número
        }
        str++;
    }
    return 1; // Es un número
}

int main()
{
    WSADATA wsaData;
    SOCKET cliente;
    struct sockaddr_in servidor_addr;
    char buffer[TAM_BUFFER];
    int opcion = -1;
    int longitud = 0;
    char opcionCaracteres[100];

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Error al inicializar Winsock\n");
        return 1;
    }

    // Creamos el socket del cliente
    cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (cliente == INVALID_SOCKET)
    {
        printf("Error al crear socket del cliente\n");
        WSACleanup();
        return 1;
    }

    // Establecemos la dirección y puerto del servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servidor_addr.sin_port = htons(PUERTO);

    // Conectamos con el servidor
    if (connect(cliente, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == SOCKET_ERROR)
    {
        printf("Error al conectar con servidor\n");
        closesocket(cliente);
        WSACleanup();
        return 1;
    }

    printf("Conexion establecida con el servidor\n");

    while (opcion != 3)
    {
        printf("----------///////////////----------\n");
        printf("Menu:\n");
        printf("1. Generar nombre de usuario\n");
        printf("2. Generar contrasena\n");
        printf("3. Salir\n");
        printf("----------///////////////----------\n");
        printf("Seleccione una opcion: ");
        scanf("%s", &opcionCaracteres);

        if (es_numero(opcionCaracteres) == 1)
        {
            opcion = atoi(opcionCaracteres);
            sprintf(buffer, "%d", opcion);
            send(cliente, buffer, strlen(buffer), 0);

            switch (opcion)
            {
            case 1:
                printf("Ingrese la longitud del nombre de usuario (5 minimo - 15 maximo): ");
                scanf("%d", &longitud);
                snprintf(buffer, sizeof(buffer), "%d", longitud);
                send(cliente, buffer, strlen(buffer), 0);
                // Recibimos nombre de usuario generado
                if(recv(cliente, buffer, TAM_BUFFER, 0) > 0){
                    printf("Respuesta del servidor: %s\n", buffer);
                } else {
                    printf("Error al recibir respuesta del servidor.\n");
                }
                break;

            case 2:
                printf("Ingrese la longitud de la contrasena (8 minimo - 50 maximo): ");
                scanf("%d", &longitud);
                snprintf(buffer,sizeof(buffer), "%d", longitud);
                send(cliente, buffer, strlen(buffer), 0);
                // Recibimos contrasenia generada
                if(recv(cliente, buffer, TAM_BUFFER, 0) > 0){
                    printf("Respuesta del servidor: %s\n", buffer);
                } else {
                    printf("Error al recibir respuesta del servidor.\n");
                }
                break;

            case 3:
                // Cerramos conexion con servidor
                snprintf(buffer,sizeof(buffer), "%d", opcion);
                send(cliente, buffer, strlen(buffer), 0);
                printf("Cerrando conexion del cliente \n");
                break;
            default:
                printf("Opcion invalida \n");
                break;
            }
        }
        else
        {
            printf("El valor ingresado no es un numero.\n");
        }
    }
    // Cerramos el socket del cliente
    closesocket(cliente);

    // Limpiamos el Winsock
    WSACleanup();

    return 0;
}