import socket
import random
import string

# Definir dirección y puerto del servidor
SERVER_IP = '127.0.0.1'
SERVER_PORT = 8081


def menu():
    print("Menu:")
    print("1. Generar nombre de usuario")
    print("2. Generar contraseña")
    print("3. Salir")


def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_IP, SERVER_PORT))

    while True:
        menu()
        opcion = input("Selecciona una opcion: ")

        if opcion == '3':
            mensaje = str(opcion)
            client_socket.sendall(mensaje.encode('utf-8'))
            print("Saliendo.")
            break

        if opcion in ['1', '2']:
            # pasamos a string la opcion para enviarsela al servidor
            mensaje = str(opcion)
            # enviamos al servidor la opcion
            client_socket.sendall(mensaje.encode('utf-8'))

            longitud = int(input("Ingrese la longitud: "))
            
            mensaje = str(longitud)
            # enviamos al servidor la longitud
            client_socket.sendall(mensaje.encode('utf-8'))

            # Recibir respuesta del servidor
            respuesta = client_socket.recv(1024)
            print('Respuesta del servidor:', respuesta.decode('utf-8'))

        else:
            print("Opción invalida, intente de nuevo.")

    client_socket.close()


if __name__ == '__main__':
    main()
