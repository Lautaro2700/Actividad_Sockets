#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <ctype.h>

#define PUERTO 8081
#define TAM_BUFFER 1024

int es_numero(const char *str)
{
    // Verificamos si la cadena está vacía
    if (*str == '\0')
    {
        return 0;
    }

    // Verificamos cada carácter en la cadena
    while (*str)
    {
        if (!isdigit((unsigned char)*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

// Función para generar un nombre de usuario de forma aleatoria
char *generarNombreUsuario(int longitud)
{
    char *nombre = (char *)malloc((longitud + 1) * sizeof(char));
    // Verificamos que el nombre no venga nulo
    if (nombre == NULL)
    {
        printf("Error al asignar memoria para el nombre de usuario\n");
        return NULL;
    }
    char vocales[] = "aeiou";
    char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int esVocal = rand() % 2; // 0 para consonante, 1 para vocal

    for (int i = 0; i < longitud; i++)
    {
        if (esVocal)
        {
            nombre[i] = vocales[rand() % strlen(vocales)];
            esVocal = 0; // Cambiamos a consonante
        }
        else
        {
            nombre[i] = consonantes[rand() % strlen(consonantes)];
            esVocal = 1; // Cambiamos a vocal
        }
    }

    nombre[longitud] = '\0'; // Terminamos la cadena
    return nombre;
}

// Función para generar contraseña de forma aleatoria
char *generarcontrasenia(int longitud)
{
    char *contrasenia = (char *)malloc((longitud + 1) * sizeof(char));
    // Verificamos que la contraseña no venga nula
    if (contrasenia == NULL)
    {
        printf("Error al asignar memoria para la contraseña\n");
        return NULL;
    }
    char mayusculas[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char minusculas[] = "abcdefghijklmnopqrstuvwxyz";
    char numeros[] = "0123456789";
    char todos[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    // Primer caracter aleatorio (mayúscula o minúscula)
    int esMayuscula = rand() % 2;
    if (esMayuscula)
    {
        contrasenia[0] = mayusculas[rand() % strlen(mayusculas)];
    }
    else
    {
        contrasenia[0] = minusculas[rand() % strlen(minusculas)];
    }

    // Resto de caracteres aleatorios
    for (int i = 1; i < longitud; i++)
    {
        contrasenia[i] = todos[rand() % strlen(todos)];
    }

    contrasenia[longitud] = '\0'; // Terminamos la cadena
    return contrasenia;
}

int main()
{
    WSADATA wsaData;
    SOCKET servidor, cliente;
    struct sockaddr_in servidor_credenciales, cliente_credenciales;
    int addr_len = sizeof(cliente_credenciales);
    char buffer[TAM_BUFFER];
    int longitud;

    // Inicializamos Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Error al inicializar Winsock\n");
        return 1;
    }

    // Creamos el socket del servidor
    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return 1;
    }

    // Establecemos la dirección y puerto del servidor
    servidor_credenciales.sin_family = AF_INET;
    servidor_credenciales.sin_addr.s_addr = INADDR_ANY;
    servidor_credenciales.sin_port = htons(PUERTO);

    // Vinculamos el socket del servidor a dirección y puerto
    if (bind(servidor, (struct sockaddr *)&servidor_credenciales, sizeof(servidor_credenciales)) == SOCKET_ERROR)
    {
        printf("Error al vincular socket del servidor\n");
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    // Escuchamos las conexiones que llegan
    if (listen(servidor, 1) == SOCKET_ERROR)
    {
        printf("Error al escuchar conexiones entrantes\n");
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    printf("Servidor escuchando en el puerto %d\n", PUERTO);

    // Aceptamos la conexión que nos llega
    cliente = accept(servidor, (struct sockaddr *)&cliente_credenciales, &addr_len);
    if (cliente == INVALID_SOCKET)
    {
        printf("Error al aceptar conexión entrante\n");
    }

    printf("Conexion establecida con cliente %s:%d\n", inet_ntoa(cliente_credenciales.sin_addr), ntohs(cliente_credenciales.sin_port));

    // Recibimos la longitud del nombre de usuario

    int opcion = 0;

    while (opcion != 3)
    {
        recv(cliente, buffer, TAM_BUFFER, 0);

        if (es_numero(buffer) == 1)
        {
            printf("Longitud correcta. Continuamos\n");
            opcion = atoi(buffer);
            switch (opcion)
            {
            case 1:
                recv(cliente, buffer, TAM_BUFFER, 0);
                longitud = atoi(buffer);
                if (longitud < 5 || longitud > 15)
                {
                    printf("ERROR: LONGITUD ERRONEA: %i\n", longitud);
                    char *mensaje = "Longitud invalida. Debe ser entre 5 y 15.";
                    send(cliente, mensaje, strlen(mensaje), 0);
                }
                else
                {
                    char *nombre = generarNombreUsuario(longitud);
                    send(cliente, nombre, strlen(nombre), 0);
                    printf("Nombre de usuario generado: %s\n", nombre);
                    free(nombre); // Liberamos memoria
                }
                break;

            case 2:
                recv(cliente, buffer, TAM_BUFFER, 0);
                longitud = atoi(buffer);

                if (longitud < 8 || longitud > 50)
                {
                    printf("ERROR: LONGITUD ERRONEA: %i\n", longitud);
                    char *mensaje = "Longitud invalida. Debe ser entre 8 y 50.";
                    send(cliente, mensaje, strlen(mensaje), 0);
                }
                else
                {
                    char *nombre = generarcontrasenia(longitud);
                    send(cliente, nombre, strlen(nombre), 0);
                    printf("Contrasenia generada: %s\n", nombre);
                    free(nombre); // Liberamos memoria
                }
                break;
            case 3:
                printf("Cerrando conexion del servidor\n");
                opcion = 3;
                break;
            default:
                printf("Opcion incorrecta.\n");
                break;
            }
        }
        else
        {
            char *mensaje = "La opcion ingresada no es un numero";
            send(cliente, mensaje, strlen(mensaje), 0);
            printf("El valor ingresado no es un numero.\n");
        }
    }
    // Cerramos el socket del servidor y del cliente
    closesocket(servidor);
    closesocket(cliente);
    // Y Limpiamos Winsock
    WSACleanup();

    return 0;
}